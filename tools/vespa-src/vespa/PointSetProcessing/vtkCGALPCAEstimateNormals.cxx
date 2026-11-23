#define CGAL_PMP_USE_CERES_SOLVER

#include "vtkCGALPCAEstimateNormals.h"

// VTK related includes
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkNew.h>

// CGAL related includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/pca_estimate_normals.h>
#include <CGAL/mst_orient_normals.h>

#include <exception>
#include <list>
#include <utility>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3                                     Point;
typedef Kernel::Vector_3                                    Vector;
typedef std::pair<Point, Vector>                            Pwn;
typedef CGAL::Parallel_if_available_tag                     Concurrency_tag;

vtkStandardNewMacro(vtkCGALPCAEstimateNormals);

//------------------------------------------------------------------------------
vtkCGALPCAEstimateNormals::vtkCGALPCAEstimateNormals()
  : NumberOfNeighbors(18)
  , OrientNormals(true)
  , DeleteUnoriented(true)
{
}

//------------------------------------------------------------------------------
void vtkCGALPCAEstimateNormals::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int vtkCGALPCAEstimateNormals::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects
  vtkPolyData* input  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  if (!input || !output)
  {
    vtkErrorMacro("Missing input or output.");
  }

  // CGAL Processing
  // ---------------

  std::list<Pwn> points;

  try
  {
    for (vtkIdType k = 0; k < input->GetNumberOfPoints(); k++)
    {
      auto pin = input->GetPoint(k);
      points.emplace_back(Point(pin[0], pin[1], pin[2]), Vector(1.0, 0.0, 0.0));
    }

    CGAL::pca_estimate_normals<Concurrency_tag>(points, this->NumberOfNeighbors,
      CGAL::parameters::point_map(CGAL::First_of_pair_property_map<Pwn>())
        .normal_map(CGAL::Second_of_pair_property_map<Pwn>()));

    if (this->OrientNormals || this->DeleteUnoriented)
    {
      // Orients normals.
      // Note: mst_orient_normals() requires a range of points
      // as well as property maps to access each point's position and normal.
      std::list<Pwn>::iterator unoriented_points_begin =
        CGAL::mst_orient_normals(points, this->NumberOfNeighbors,
          CGAL::parameters::point_map(CGAL::First_of_pair_property_map<Pwn>())
            .normal_map(CGAL::Second_of_pair_property_map<Pwn>()));

      if (this->DeleteUnoriented)
      {
        // Optional: delete points with an unoriented normal
        // if you plan to call a reconstruction algorithm that expects oriented normals.
        points.erase(unoriented_points_begin, points.end());
      }
    }
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // VTK Output
  // ----------

  vtkNew<vtkPolyData> polydata;
  vtkNew<vtkPoints>   outpoints;

  vtkNew<vtkDoubleArray> pointNormalsArray;
  pointNormalsArray->SetName("Normals");
  pointNormalsArray->SetNumberOfComponents(3); // 3d normals (ie x,y,z)

  for (const auto& p : points)
  {
    outpoints->InsertNextPoint(p.first[0], p.first[1], p.first[2]);
    pointNormalsArray->InsertNextTuple3(p.second[0], p.second[1], p.second[2]);
  }

  polydata->SetPoints(outpoints);
  polydata->GetPointData()->AddArray(pointNormalsArray);
  polydata->GetPointData()->SetNormals(pointNormalsArray);

  output->ShallowCopy(polydata);
  // this->copyAttributes(input, output);

  return 1;
}
