#define CGAL_PMP_USE_CERES_SOLVER

#include "vtkCGALPoissonSurfaceReconstructionDelaunay.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyDataNormals.h>

// CGAL related includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/poisson_surface_reconstruction.h>
#include <CGAL/version.h>
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(6,1,0)
#  include <CGAL/Poisson_mesh_domain_3.h>
#  include <CGAL/Mesh_triangulation_3.h>
#  include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#  include <CGAL/Mesh_criteria_3.h>
#  include <CGAL/make_mesh_3.h>
#  include <CGAL/facets_in_complex_3_to_triangle_mesh.h>
#else
#  include <CGAL/Polyhedron_3.h>
#  include <CGAL/Surface_mesh_default_triangulation_3.h>
#  include <CGAL/Surface_mesh_complex_2_in_triangulation_3.h>
#  include <CGAL/Implicit_surface_3.h>
#  include <CGAL/IO/facets_in_complex_2_to_triangle_mesh.h>
#endif
#include <CGAL/Surface_mesh.h>

#include <exception>
#include <memory>
#include <utility>
#include <vector>

typedef CGAL_Kernel::FT                                      FT;
typedef CGAL_Kernel::Point_3                                 Point;
typedef CGAL_Kernel::Vector_3                                Vector;
typedef CGAL_Kernel::Sphere_3                                Sphere;
typedef std::pair<Point, Vector>                             Pwn;
typedef CGAL::First_of_pair_property_map<Pwn>                Point_map;
typedef CGAL::Second_of_pair_property_map<Pwn>               Normal_map;
typedef CGAL::Poisson_reconstruction_function<CGAL_Kernel>   Poisson_reconstruction_function;
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(6,1,0)
typedef CGAL::Poisson_mesh_domain_3<CGAL_Kernel>             Mesh_domain;
typedef CGAL::Mesh_triangulation_3<Mesh_domain, CGAL::Default, CGAL::Sequential_tag>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr, int, int> C3t3;
typedef CGAL::Mesh_criteria_3<Tr>                            Mesh_criteria;
#else
typedef CGAL::Surface_mesh_default_triangulation_3           STr;
typedef CGAL::Surface_mesh_complex_2_in_triangulation_3<STr> C2t3;
typedef CGAL::Implicit_surface_3<CGAL_Kernel, Poisson_reconstruction_function> Surface_3;
#endif

vtkStandardNewMacro(vtkCGALPoissonSurfaceReconstructionDelaunay);

//------------------------------------------------------------------------------
vtkCGALPoissonSurfaceReconstructionDelaunay::vtkCGALPoissonSurfaceReconstructionDelaunay()
  : MinTriangleAngle(20.0)
  , MaxTriangleSize(2.0)
  , Distance(0.375)
  , GenerateSurfaceNormals(true)
{
}

//------------------------------------------------------------------------------
void vtkCGALPoissonSurfaceReconstructionDelaunay::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int vtkCGALPoissonSurfaceReconstructionDelaunay::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects
  vtkPolyData* input  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  if (!input || !output)
  {
    vtkErrorMacro("Missing input or output.");
  }

  // Create the surface mesh for CGAL
  // ----------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  vtkCGALHelper::toCGAL(input, cgalMesh.get());

  // CGAL Processing
  // ---------------

  // Poisson options
  FT sm_angle    = this->MinTriangleAngle; // Min triangle angle in degrees.
  FT sm_radius   = this->MaxTriangleSize;  // Max triangle size w.r.t. point set average spacing.
  FT sm_distance = this->Distance; // Surface Approximation error w.r.t. point set average spacing.

  auto normals = input->GetPointData()->GetNormals();

  if (!normals)
  {
    vtkErrorMacro("Point normals required.");
    return 0;
  }

  try
  {
    std::vector<Pwn> points;

    for (vtkIdType k = 0; k < input->GetNumberOfPoints(); k++)
    {
      auto pin = input->GetPoint(k);
      auto vin = normals->GetTuple3(k);
      points.emplace_back(Point(pin[0], pin[1], pin[2]), Vector(vin[0], vin[1], vin[2]));
    }

    // Note: this method requires an iterator over points
    // + property maps to access each point's position and normal.
    Poisson_reconstruction_function function(
      points.begin(), points.end(), Point_map(), Normal_map());

    // Computes the Poisson indicator function f()
    // at each vertex of the triangulation.
    if (!function.compute_implicit_function())
    {
      vtkErrorMacro("Failed to compute the Poisson indicator function.");
      return 0;
    }

    // Computes average spacing
    FT average_spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag>(
      points, 6 /* knn = 1 ring */, CGAL::parameters::point_map(Point_map()));
    // Gets one point inside the implicit surface
    // and computes implicit function bounding sphere radius.
    Point  inner_point = function.get_inner_point();
    Sphere bsphere     = function.bounding_sphere();
    FT     radius      = std::sqrt(bsphere.squared_radius());
    FT sm_sphere_radius  = 5.0 * radius;
    FT sm_dichotomy_error = sm_distance * average_spacing / 1000.0;

#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(6,1,0)
    Mesh_domain domain(function,
      Sphere(inner_point, sm_sphere_radius * sm_sphere_radius),
      CGAL::parameters::relative_error_bound(sm_dichotomy_error / sm_sphere_radius));

    Mesh_criteria criteria(CGAL::parameters::facet_angle(sm_angle)
                                            .facet_size(sm_radius * average_spacing)
                                            .facet_distance(sm_distance * average_spacing));

    C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria,
      CGAL::parameters::no_perturb().no_exude());

    CGAL::facets_in_complex_3_to_triangle_mesh(c3t3, cgalMesh->surface);
#else
    Surface_3 surface(function,
      Sphere(inner_point, sm_sphere_radius * sm_sphere_radius),
      sm_dichotomy_error / sm_sphere_radius);

    CGAL::Surface_mesh_default_criteria_3<STr> criteria(sm_angle,
      sm_radius * average_spacing,
      sm_distance * average_spacing);

    STr  tr;
    C2t3 c2t3(tr);
    CGAL::make_surface_mesh(c2t3, surface, criteria, CGAL::Manifold_with_boundary_tag());

    CGAL::facets_in_complex_2_to_triangle_mesh(c2t3, cgalMesh->surface);
#endif
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // VTK Output
  // ----------

  vtkNew<vtkPolyData> mesh;
  vtkCGALHelper::toVTK(cgalMesh.get(), mesh);
  this->copyAttributes(input, mesh);

  if (this->GenerateSurfaceNormals)
  {
    vtkNew<vtkPolyDataNormals> normals;
    normals->SetInputData(mesh);
    normals->ComputePointNormalsOn();
    normals->ConsistencyOn();
    normals->SplittingOff();
    normals->Update();

    output->ShallowCopy(normals->GetOutput());
  }
  else
    output->ShallowCopy(mesh);

  return 1;
}
