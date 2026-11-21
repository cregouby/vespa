#include "vtkCGALMeshDeformation.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkExtractSelection.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPointSet.h>
#include <vtkSelection.h>

// CGAL related includes
#include <CGAL/Surface_mesh_deformation.h>

#include <exception>
#include <memory>
#include <vector>

vtkStandardNewMacro(vtkCGALMeshDeformation);

using SmoothDeformation = CGAL::Surface_mesh_deformation<CGAL_Surface>;
using SRE_ARAPDeformation =
  CGAL::Surface_mesh_deformation<CGAL_Surface, CGAL::Default, CGAL::Default, CGAL::SRE_ARAP>;

//------------------------------------------------------------------------------
vtkCGALMeshDeformation::vtkCGALMeshDeformation()
{
  this->SetNumberOfInputPorts(3);
}

//------------------------------------------------------------------------------
void vtkCGALMeshDeformation::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "Mode : " << this->Mode << std::endl;
  os << indent << "SreAlpha : " << this->SreAlpha << std::endl;
  os << indent << "Number of Iterations :" << this->NumberOfIterations << std::endl;
  os << indent << "Tolerance :" << this->Tolerance << std::endl;
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int vtkCGALMeshDeformation::FillInputPortInformation(int port, vtkInformation* info)
{
  if (port == 0)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
  }
  else if (port == 1)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPointSet");
  }
  else
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkSelection");
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
  }

  return 1;
}

//------------------------------------------------------------------------------
void vtkCGALMeshDeformation::SetSourceConnection(vtkAlgorithmOutput* algOutput)
{
  this->SetInputConnection(1, algOutput);
}

//------------------------------------------------------------------------------
void vtkCGALMeshDeformation::SetSelectionConnection(vtkAlgorithmOutput* algOutput)
{
  this->SetInputConnection(2, algOutput);
}

//------------------------------------------------------------------------------
int vtkCGALMeshDeformation::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects.
  vtkPolyData* input   = vtkPolyData::GetData(inputVector[0]);
  vtkPointSet* targets = vtkPointSet::GetData(inputVector[1]);
  vtkPolyData* output  = vtkPolyData::GetData(outputVector);

  if (!input || !targets || !output)
  {
    vtkErrorMacro(<< "Missing input or output!");
    return 0;
  }

  // Get the optional selection input
  vtkInformation*     selInfo = inputVector[2]->GetInformationObject(0);
  vtkNew<vtkPointSet> roi;

  // If no selection is given, use control points as ROI
  // A better approach could use all points within a radius
  // of the control points to avoid self intersections
  if (selInfo)
  {
    vtkSelection* roiSel = vtkSelection::SafeDownCast(selInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkNew<vtkExtractSelection> extractSelection;
    extractSelection->SetInputData(0, input);
    extractSelection->SetInputData(1, roiSel);
    extractSelection->Update();
    roi->ShallowCopy(extractSelection->GetOutputDataObject(0));
    if (!roi || roi->GetNumberOfPoints() == 0)
    {
      vtkErrorMacro("Not a valid selection, need points.");
      output->ShallowCopy(input);
      return 0;
    }
  }
  else
  {
    roi->ShallowCopy(targets);
  }

  // Create the triangle mesh for CGAL
  // --------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  vtkCGALHelper::toCGAL(input, cgalMesh.get());

  // Create the deformation object
  // ---------------------------------

  SmoothDeformation   smoothDeformer(cgalMesh->surface);
  SRE_ARAPDeformation arapDeformer(cgalMesh->surface);
  arapDeformer.set_sre_arap_alpha(this->SreAlpha);

  // Find global ID array name
  // ---------------------------------

  if (this->GlobalIdArray.empty())
  {
    if (!input->GetPointData()->GetGlobalIds())
    {
      vtkErrorMacro(
        << "Could not find a default array for global IDs. Please specify an array name "
           "using SetGlobalIdArray().");
      return 0;
    }
    else
    {
      this->GlobalIdArray = input->GetPointData()->GetGlobalIds()->GetName();
    }
  }

  // Define the ROI
  // ---------------------------------

  if (!roi->GetPointData()->GetArray(this->GlobalIdArray.c_str()))
  {
    vtkErrorMacro(<< "No array named " << this->GlobalIdArray << " for the ROI!");
    return 0;
  }

  auto gids =
    vtk::DataArrayValueRange<1>(roi->GetPointData()->GetArray(this->GlobalIdArray.c_str()));
  std::vector<Graph_Verts> roiVerts(gids.cbegin(), gids.cend());
  try
  {
    switch (this->Mode)
    {
      case SRE_ARAP:
        arapDeformer.insert_roi_vertices(roiVerts.begin(), roiVerts.end());
        break;
      default:
        smoothDeformer.insert_roi_vertices(roiVerts.begin(), roiVerts.end());
        break;
    }
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // Define the control points
  // ---------------------------------

  if (!targets->GetPointData()->GetArray(this->GlobalIdArray.c_str()))
  {
    vtkErrorMacro(<< "No array named " << this->GlobalIdArray << " for the control points!");
    return 0;
  }

  gids =
    vtk::DataArrayValueRange<1>(targets->GetPointData()->GetArray(this->GlobalIdArray.c_str()));
  std::vector<Graph_Verts> ctrlPoints(gids.cbegin(), gids.cend());
  try
  {
    switch (this->Mode)
    {
      case SRE_ARAP:
        arapDeformer.insert_control_vertices(ctrlPoints.begin(), ctrlPoints.end());
        break;
      default:
        smoothDeformer.insert_control_vertices(ctrlPoints.begin(), ctrlPoints.end());
        break;
    }
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // Define the target positions for the control points
  // ---------------------------------

  for (vtkIdType ptIdx = 0; ptIdx < targets->GetNumberOfPoints(); ++ptIdx)
  {
    double coords[3] = { 0.0, 0.0, 0.0 };
    targets->GetPoint(ptIdx, coords);
    try
    {
      switch (this->Mode)
      {
        case SRE_ARAP:
        {
          SRE_ARAPDeformation::Point targetPosARAP(coords[0], coords[1], coords[2]);
          arapDeformer.set_target_position(ctrlPoints[ptIdx], targetPosARAP);
          break;
        }
        default:
        {
          SmoothDeformation::Point targetPosSmooth(coords[0], coords[1], coords[2]);
          smoothDeformer.set_target_position(ctrlPoints[ptIdx], targetPosSmooth);
          break;
        }
      }
    }
    catch (std::exception& e)
    {
      vtkErrorMacro("CGAL Exception: " << e.what());
      return 0;
    }
  }

  // CGAL Processing
  // ---------------

  try
  {
    // Deform mesh given ROI and control point targets
    switch (this->Mode)
    {
      case SRE_ARAP:
        arapDeformer.deform(this->NumberOfIterations, this->Tolerance);
        break;
      default:
        smoothDeformer.deform(this->NumberOfIterations, this->Tolerance);
        break;
    }
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

  return 1;
}
