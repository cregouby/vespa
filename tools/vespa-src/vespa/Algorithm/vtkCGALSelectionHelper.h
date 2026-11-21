#ifndef VESPA_CGAL_SELECTION_HELPER_H
#define VESPA_CGAL_SELECTION_HELPER_H

#include <vtkCell.h>
#include <vtkIdList.h>
#include <vtkIdTypeArray.h>
#include <vtkInformation.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkSmartPointer.h>

#include <vector>

inline std::vector<vtkIdType> vtkCGALSelectedPointIds(vtkSelection* selection)
{
  std::vector<vtkIdType> ids;
  if (!selection) return ids;

  for (vtkIdType n = 0; n < selection->GetNumberOfNodes(); ++n)
  {
    vtkSelectionNode* node = selection->GetNode(n);
    if (!node) continue;

    const int contentType = node->GetProperties()->Get(vtkSelectionNode::CONTENT_TYPE());
    const int fieldType = node->GetProperties()->Get(vtkSelectionNode::FIELD_TYPE());
    if (contentType != vtkSelectionNode::INDICES || fieldType != vtkSelectionNode::POINT) continue;

    vtkIdTypeArray* list = vtkIdTypeArray::SafeDownCast(node->GetSelectionList());
    if (!list) continue;

    for (vtkIdType i = 0; i < list->GetNumberOfTuples(); ++i)
      ids.push_back(list->GetValue(i));
  }
  return ids;
}

inline vtkSmartPointer<vtkPolyData> vtkCGALRemovePoints(
  vtkPolyData* input, const std::vector<vtkIdType>& pointIds)
{
  vtkSmartPointer<vtkPolyData> result = vtkSmartPointer<vtkPolyData>::New();
  if (!input) return result;

  const vtkIdType nPoints = input->GetNumberOfPoints();
  std::vector<bool> selected(nPoints, false);
  for (vtkIdType id : pointIds)
    if (id >= 0 && id < nPoints) selected[id] = true;

  vtkNew<vtkIdList> cells;
  for (vtkIdType cid = 0; cid < input->GetNumberOfCells(); ++cid)
  {
    vtkCell* cell = input->GetCell(cid);
    bool keep = true;
    for (vtkIdType i = 0; i < cell->GetNumberOfPoints(); ++i)
    {
      if (selected[cell->GetPointId(i)])
      {
        keep = false;
        break;
      }
    }
    if (keep) cells->InsertNextId(cid);
  }

  vtkNew<vtkPoints> points;
  std::vector<vtkIdType> remap(nPoints, -1);
  for (vtkIdType i = 0; i < cells->GetNumberOfIds(); ++i)
  {
    vtkCell* cell = input->GetCell(cells->GetId(i));
    for (vtkIdType j = 0; j < cell->GetNumberOfPoints(); ++j)
    {
      const vtkIdType pid = cell->GetPointId(j);
      if (remap[pid] < 0)
      {
        double coords[3];
        input->GetPoint(pid, coords);
        remap[pid] = points->InsertNextPoint(coords);
      }
    }
  }

  result->SetPoints(points);
  result->Allocate(cells->GetNumberOfIds());
  for (vtkIdType i = 0; i < cells->GetNumberOfIds(); ++i)
  {
    vtkCell* cell = input->GetCell(cells->GetId(i));
    vtkNew<vtkIdList> newIds;
    for (vtkIdType j = 0; j < cell->GetNumberOfPoints(); ++j)
      newIds->InsertNextId(remap[cell->GetPointId(j)]);
    result->InsertNextCell(cell->GetCellType(), newIds);
  }
  return result;
}

#endif
