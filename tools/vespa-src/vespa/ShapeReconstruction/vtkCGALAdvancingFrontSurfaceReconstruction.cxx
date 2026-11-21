#define CGAL_PMP_USE_CERES_SOLVER

#include "vtkCGALAdvancingFrontSurfaceReconstruction.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

// CGAL related includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Advancing_front_surface_reconstruction.h>

#include <array>
#include <iterator>
#include <memory>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_3                                          Point_3;
typedef std::array<std::size_t, 3>                          Facet;

vtkStandardNewMacro(vtkCGALAdvancingFrontSurfaceReconstruction);

namespace pmp = CGAL::Polygon_mesh_processing;

//------------------------------------------------------------------------------
vtkCGALAdvancingFrontSurfaceReconstruction::vtkCGALAdvancingFrontSurfaceReconstruction()
  : Per(0.0)
  , RadiusRatioBound(5.0)
{
}

//------------------------------------------------------------------------------
void vtkCGALAdvancingFrontSurfaceReconstruction::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
struct Perimeter
{
  double bound;
  Perimeter(double bound)
    : bound(bound)
  {
  }
  template <typename AdvancingFront, typename Cell_handle>
  double operator()(const AdvancingFront& adv, Cell_handle& c, const int& index) const
  {
    // bound == 0 is better than bound < infinity
    // as it avoids the distance computations
    if (bound == 0)
    {
      return adv.smallest_radius_delaunay_sphere(c, index);
    }
    // If perimeter > bound, return infinity so that facet is not used
    double d = 0;
    d        = sqrt(
      squared_distance(c->vertex((index + 1) % 4)->point(), c->vertex((index + 2) % 4)->point()));
    if (d > bound)
      return adv.infinity();
    d += sqrt(
      squared_distance(c->vertex((index + 2) % 4)->point(), c->vertex((index + 3) % 4)->point()));
    if (d > bound)
      return adv.infinity();
    d += sqrt(
      squared_distance(c->vertex((index + 1) % 4)->point(), c->vertex((index + 3) % 4)->point()));
    if (d > bound)
      return adv.infinity();
    // Otherwise, return usual priority value: smallest radius of
    // delaunay sphere
    return adv.smallest_radius_delaunay_sphere(c, index);
  }
};

//------------------------------------------------------------------------------
int vtkCGALAdvancingFrontSurfaceReconstruction::RequestData(
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

  std::vector<Facet> facets;

  try
  {
    std::vector<Point_3> points;

    for (vtkIdType k = 0; k < input->GetNumberOfPoints(); k++)
    {
      auto pin = input->GetPoint(k);
      points.emplace_back(pin[0], pin[1], pin[2]);
    }

    Perimeter perimeter(this->Per);
    CGAL::advancing_front_surface_reconstruction(
      points.begin(), points.end(), std::back_inserter(facets), perimeter, this->RadiusRatioBound);
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // VTK Output
  // ----------

  vtkCGALHelper::toVTK(cgalMesh.get(), output);
  this->copyAttributes(input, output);

  for (auto facet : facets)
  {
    vtkNew<vtkIdList> tri;
    tri->InsertNextId(facet[0]);
    tri->InsertNextId(facet[1]);
    tri->InsertNextId(facet[2]);

    output->InsertNextCell(VTK_TRIANGLE, tri);
  }

  return 1;
}
