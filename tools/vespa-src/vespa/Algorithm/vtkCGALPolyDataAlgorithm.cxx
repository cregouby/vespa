#include "vtkCGALPolyDataAlgorithm.h"

// VTK related includes
#include <vtkCellData.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyDataNormals.h>
#include <vtkProbeFilter.h>

#include <iostream>

vtkStandardNewMacro(vtkCGALPolyDataAlgorithm);

//------------------------------------------------------------------------------
void vtkCGALPolyDataAlgorithm::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "UpdateAttributes:" << this->UpdateAttributes << std::endl;
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
bool vtkCGALPolyDataAlgorithm::interpolateAttributes(vtkPolyData* input, vtkPolyData* vtkMesh)
{
  if (this->UpdateAttributes)
  {
    vtkNew<vtkProbeFilter> probe;
    probe->SetInputData(vtkMesh);
    probe->SetSourceData(input);
    probe->SpatialMatchOn();
    probe->Update();

    vtkMesh->ShallowCopy(probe->GetOutput());
  }
  return true;
}

//------------------------------------------------------------------------------
bool vtkCGALPolyDataAlgorithm::copyAttributes(vtkPolyData* input, vtkPolyData* vtkMesh)
{
  if (this->UpdateAttributes)
  {
    vtkMesh->GetPointData()->ShallowCopy(input->GetPointData());
    vtkMesh->GetCellData()->ShallowCopy(input->GetCellData());
  }

  return true;
}
