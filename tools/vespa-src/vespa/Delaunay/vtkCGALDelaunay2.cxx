#include "vtkCGALDelaunay2.h"

// VTK related includes
#include <vtkCellArrayIterator.h>
#include <vtkDataSet.h>
#include <vtkIdList.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

// CGAL related includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>

#include <list>
#include <map>
#include <vector>

using CGAL_Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;

vtkStandardNewMacro(vtkCGALDelaunay2);

// TODO May try to use ProjectionTraits_3 to handle open 3D surfaces
// Look at perf then
using CDT2 = CGAL::Constrained_Delaunay_triangulation_2<CGAL_Kernel, CGAL::Default,
  CGAL::No_constraint_intersection_requiring_constructions_tag>;

//------------------------------------------------------------------------------
void vtkCGALDelaunay2::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int vtkCGALDelaunay2::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects.
  vtkPolyData* input  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  // Create the surface mesh for CGAL
  // --------------------------------

  vtkPoints*    vtkPts     = input->GetPoints();
  vtkIdType     nbPts      = input->GetNumberOfPoints();
  vtkDataArray* ptsArr     = vtkPts->GetData();
  const auto    pointRange = vtk::DataArrayTupleRange<3>(ptsArr);

  // manually handle the planar coordinate
  // should be along the x, y or z axis
  double rangeVal[3];
  for (int i = 0; i < 3; i++)
  {
    double range[2];
    ptsArr->GetRange(range, i);
    rangeVal[i] = range[1] - range[0];
  }

  if (rangeVal[0] && rangeVal[1] && rangeVal[2])
  {
    vtkErrorMacro("This dataset is 3D");
  }
  int d1 = 0, d2 = 1, d3 = 2; // assume z is null
  if (!rangeVal[0])           // x is null
  {
    d1 = 1;
    d2 = 2;
    d3 = 0;
  }
  if (!rangeVal[1]) // y is null
  {
    d1 = 0;
    d2 = 2;
    d3 = 1;
  }

  std::vector<CDT2::Point> pts;
  pts.reserve(nbPts);
  for (const auto& pt : pointRange)
  {
    pts.emplace_back(pt[d1], pt[d2]);
  }

  // CGAL Processing
  // ---------------

  CDT2 delaunay;
  try
  {
    // Add constraints (lines and polys)
    vtkCellArray* polys   = input->GetPolys();
    auto          polysIt = vtk::TakeSmartPointer(polys->NewIterator());
    // each poly
    for (polysIt->GoToFirstCell(); !polysIt->IsDoneWithTraversal(); polysIt->GoToNextCell())
    {
      vtkIdList*             p = polysIt->GetCurrentCell();
      std::list<CDT2::Point> poly;
      // each segment of poly
      for (vtkIdType i = 0; i < p->GetNumberOfIds(); i++)
      {
        poly.emplace_back(pts[p->GetId(i)]);
      }

      try
      {
        delaunay.insert_constraint(poly.begin(), poly.end(), true /*close*/);
      }
      catch (const CDT2::Intersection_of_constraints_exception& e)
      {
        // If we have an invalid constraint (such as edges intersecting in at least
        // one of the edges' interiors), we just ignore the constraint and continue
        vtkWarningMacro("Ill-formed constraint detected: constraint ignored.");
        continue;
      }
    }

    vtkCellArray* lines   = input->GetLines();
    auto          linesIt = vtk::TakeSmartPointer(lines->NewIterator());
    // each line
    for (linesIt->GoToFirstCell(); !linesIt->IsDoneWithTraversal(); linesIt->GoToNextCell())
    {
      // each segment of line
      vtkIdList*             l = linesIt->GetCurrentCell();
      std::list<CDT2::Point> line;
      for (vtkIdType i = 1; i < l->GetNumberOfIds(); i++)
      {
        line.emplace_back(pts[l->GetId(i)]);
      }

      try
      {
        delaunay.insert_constraint(line.begin(), line.end(), false /*close*/);
      }
      catch (const CDT2::Intersection_of_constraints_exception& e)
      {
        // If we have an invalid constraint (such as edges intersecting in at least
        // one of the edges' interiors), we just ignore the constraint and continue
        vtkWarningMacro("Ill-formed constraint detected: constraint ignored.");
        continue;
      }
    }

    // Add points
    for (const auto& point : pts)
    {
      delaunay.push_back(CDT2::Point(point));
    }
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // VTK Output
  // ----------

  vtkNew<vtkPoints> outPts;
  const vtkIdType   outNPts = delaunay.number_of_vertices();
  outPts->Allocate(outNPts);
  std::map<CDT2::Point, vtkIdType> vmap;

  for (auto vertex : delaunay.finite_vertex_handles())
  {
    double coords[3];
    coords[d1]                   = vertex->point()[0];
    coords[d2]                   = vertex->point()[1];
    coords[d3]                   = rangeVal[d3];
    vtkIdType id                 = outPts->InsertNextPoint(coords);
    vmap[delaunay.point(vertex)] = id;
  }
  outPts->Squeeze();

  // cells
  vtkNew<vtkCellArray> cells;
  cells->AllocateEstimate(delaunay.number_of_faces(), 3);

  for (auto face : delaunay.finite_face_handles())
  {
    vtkNew<vtkIdList> ids;
    ids->InsertNextId(vmap[delaunay.point(face, 0)]);
    ids->InsertNextId(vmap[delaunay.point(face, 1)]);
    ids->InsertNextId(vmap[delaunay.point(face, 2)]);

    cells->InsertNextCell(ids);
  }
  cells->Squeeze();

  // VTK dataset
  output->SetPoints(outPts);
  output->SetPolys(cells);

  return 1;
}
