#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkCellIterator.h>
#include <vtkPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkSetGet.h>

//------------------------------------------------------------------------------
bool vtkCGALHelper::toCGAL(vtkPolyData* vtkMesh, vtkCGALHelper::Vespa_soup* cgalMesh)
{

  // Points
  const vtkIdType inNPts = vtkMesh->GetNumberOfPoints();
  cgalMesh->points.clear();
  cgalMesh->points.reserve(inNPts);

  for (vtkIdType pid = 0; pid < inNPts; pid++)
  {
    double coords[3];
    vtkMesh->GetPoint(pid, coords);
    cgalMesh->points.emplace_back(coords[0], coords[1], coords[2]);
  }

  // Cells
  const vtkIdType inNCells = vtkMesh->GetNumberOfCells();
  cgalMesh->faces.clear();
  cgalMesh->faces.resize(inNCells);

  vtkIdType cid = 0;
  auto      cit = vtk::TakeSmartPointer(vtkMesh->NewCellIterator());
  for (cit->InitTraversal(); !cit->IsDoneWithTraversal(); cit->GoToNextCell(), cid++)
  {
    // Add the cell
    vtkIdList* ids   = cit->GetPointIds();
    vtkIdType  nbIds = cit->GetNumberOfPoints();

    cgalMesh->faces[cid].reserve(ids->GetNumberOfIds());
    for (vtkIdType i = 0; i < nbIds; i++)
    {
      cgalMesh->faces[cid].emplace_back(ids->GetId(i));
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool vtkCGALHelper::toCGAL(vtkPolyData* vtkMesh, vtkCGALHelper::Vespa_surface* cgalMesh)
{
  bool status = true;

  // preprocess: ensure cell consistency in VTK
  // this is required by CGAL
  vtkNew<vtkPolyDataNormals> consistency;
  consistency->SetInputData(vtkMesh);
  consistency->SplittingOff();
  consistency->NonManifoldTraversalOff();
  consistency->Update();
  vtkMesh = consistency->GetOutput(0);

  // Vertices
  const vtkIdType inNPts = vtkMesh->GetNumberOfPoints();

  std::vector<Graph_Verts> surfaceVertices(inNPts);
  for (vtkIdType i = 0; i < inNPts; i++)
  {
    // id
    surfaceVertices[i] = add_vertex(cgalMesh->surface);

    // coords
    double coords[3];
    vtkMesh->GetPoint(i, coords);
    put(
      cgalMesh->coords, surfaceVertices[i], CGAL_Kernel::Point_3(coords[0], coords[1], coords[2]));
  }

  // Cells
  auto cit = vtk::TakeSmartPointer(vtkMesh->NewCellIterator());
  for (cit->InitTraversal(); !cit->IsDoneWithTraversal(); cit->GoToNextCell())
  {
    // Add the cell
    vtkIdList* ids   = cit->GetPointIds();
    vtkIdType  nbIds = cit->GetNumberOfPoints();

    std::vector<Graph_Verts> cell(nbIds);
    for (vtkIdType i = 0; i < nbIds; i++)
    {
      cell[i] = surfaceVertices[ids->GetId(i)];
    }

    // Fails on non-manifold cells
    auto newFace = CGAL::Euler::add_face(cell, cgalMesh->surface);
    status &= newFace.is_valid();
  }

  if (!status)
  {
    vtkWarningWithObjectMacro(nullptr,
      "Invalid cell detected, CGAL may have an incomplete input mesh."
      "This is likely due to a non-manifold input."
      "You may want to try the Vespa Mesh Checker filter.");
  }

  return status;
}

//------------------------------------------------------------------------------
bool vtkCGALHelper::toVTK(vtkCGALHelper::Vespa_soup const* cgalMesh, vtkPolyData* vtkMesh)
{
  // points (vertices in surfaceMesh are not contiguous)
  vtkNew<vtkPoints> pts;
  const vtkIdType   outNPts = cgalMesh->points.size();
  pts->Allocate(outNPts);

  for (const auto& point : cgalMesh->points)
  {
    pts->InsertNextPoint(point[0], point[1], point[2]);
  }
  pts->Squeeze();

  // cells
  vtkNew<vtkCellArray> cells;
  cells->AllocateEstimate(cgalMesh->faces.size(), 3);

  for (const auto& face : cgalMesh->faces)
  {
    vtkNew<vtkIdList> ids;
    ids->Allocate(face.size());

    for (auto id : face)
    {
      ids->InsertNextId(id);
    }
    cells->InsertNextCell(ids);
  }
  cells->Squeeze();

  // VTK dataset
  vtkMesh->Reset(); // always start from new mesh
  vtkMesh->SetPoints(pts);
  vtkMesh->SetPolys(cells);

  return true;
}

//------------------------------------------------------------------------------
bool vtkCGALHelper::toVTK(vtkCGALHelper::Vespa_surface const* cgalMesh, vtkPolyData* vtkMesh)
{
  // points (vertices in surfaceMesh are not contiguous)
  vtkNew<vtkPoints> pts;
  const vtkIdType   outNPts = num_vertices(cgalMesh->surface);
  pts->Allocate(outNPts);
  std::vector<vtkIdType> vmap(outNPts);

  for (auto vertex : vertices(cgalMesh->surface))
  {
    const auto& p = get(cgalMesh->coords, vertex);
    vtkIdType   id =
      pts->InsertNextPoint(CGAL::to_double(p.x()), CGAL::to_double(p.y()), CGAL::to_double(p.z()));
    vmap[vertex] = id;
  }
  pts->Squeeze();

  // cells
  vtkNew<vtkCellArray> cells;
  cells->AllocateEstimate(num_faces(cgalMesh->surface), 3);

  for (auto face : faces(cgalMesh->surface))
  {
    vtkNew<vtkIdList> ids;
    ids->Allocate(3);
    for (auto edge : halfedges_around_face(halfedge(face, cgalMesh->surface), cgalMesh->surface))
    {
      ids->InsertNextId(vmap[source(edge, cgalMesh->surface)]);
    }
    cells->InsertNextCell(ids);
  }
  cells->Squeeze();

  // VTK dataset
  vtkMesh->Reset(); // always start from new mesh
  vtkMesh->SetPoints(pts);
  vtkMesh->SetPolys(cells);

  return true;
}
