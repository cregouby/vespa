#include "vtkCGALRegionFairing.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include "vtkCGALSelectionHelper.h"
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkSelection.h>

// CGAL related includes
#include <CGAL/Polygon_mesh_processing/fair.h>

#include <exception>
#include <memory>
#include <vector>

vtkStandardNewMacro(vtkCGALRegionFairing);

namespace pmp = CGAL::Polygon_mesh_processing;

//------------------------------------------------------------------------------
vtkCGALRegionFairing::vtkCGALRegionFairing()
{
  this->SetNumberOfInputPorts(2);
}

//------------------------------------------------------------------------------
void vtkCGALRegionFairing::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int vtkCGALRegionFairing::FillInputPortInformation(int port, vtkInformation* info)
{
  if (port == 0)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
  }
  else
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkSelection");
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
  }
  return 1;
}

//------------------------------------------------------------------------------
void vtkCGALRegionFairing::SetSourceConnection(vtkAlgorithmOutput* algOutput)
{
  this->SetInputConnection(1, algOutput);
}

//------------------------------------------------------------------------------
int vtkCGALRegionFairing::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects.
  vtkPolyData* input  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  // Get the selection input
  vtkInformation* selInfo = inputVector[1]->GetInformationObject(0);
  if (!selInfo)
  {
    // When not given a selection, nothing to do.
    vtkWarningMacro("No selection made, nothing to do");
    output->ShallowCopy(input);
    return 1;
  }
  vtkSelection* inputSel = vtkSelection::SafeDownCast(selInfo->Get(vtkDataObject::DATA_OBJECT()));
  const std::vector<vtkIdType> sel = vtkCGALSelectedPointIds(inputSel);
  if (sel.empty())
  {
    vtkErrorMacro("Not a valid selection, need points.");
    output->ShallowCopy(input);
    return 0;
  }

  // Create the triangle mesh for CGAL
  // --------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  vtkCGALHelper::toCGAL(input, cgalMesh.get());

  // Retrieve the region to fair (ROI)
  // ---------------------------------
  std::vector<Graph_Verts> vertices;
  vertices.reserve(sel.size());
  for (vtkIdType id : sel)
    vertices.push_back(static_cast<Graph_Verts>(id));

  // CGAL Processing
  // ---------------

  try
  {
    // fair selected area
    pmp::fair(cgalMesh->surface, vertices);
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // VTK Output
  // ----------

  vtkCGALHelper::toVTK(cgalMesh.get(), output);
  this->interpolateAttributes(input, output);

  return 1;
}
