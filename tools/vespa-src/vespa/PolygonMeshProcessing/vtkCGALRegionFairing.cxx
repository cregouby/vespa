#include "vtkCGALRegionFairing.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkExtractSelection.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>

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
  vtkNew<vtkExtractSelection> extractSelection;
  extractSelection->SetInputData(0, input);
  extractSelection->SetInputData(1, inputSel);
  extractSelection->Update();
  vtkPointSet* dataSel = vtkPointSet::SafeDownCast(extractSelection->GetOutputDataObject(0));
  if (!dataSel || dataSel->GetNumberOfPoints() == 0)
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
  auto gids = vtk::DataArrayValueRange(dataSel->GetPointData()->GetArray("vtkOriginalPointIds"));
  std::vector<Graph_Verts> sel(gids.cbegin(), gids.cend());

  // CGAL Processing
  // ---------------

  try
  {
    // fair selected area
    pmp::fair(cgalMesh->surface, sel);
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
