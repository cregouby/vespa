#include "vtkCGALPatchFilling.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkDataSetSurfaceFilter.h>
#include <vtkExtractSelection.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkTriangleFilter.h>
#include <vtkThreshold.h>

// CGAL related includes
#include <CGAL/version.h>
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(6,2,0)
#  include <CGAL/boost/graph/border.h>
#else
#  include <CGAL/Polygon_mesh_processing/border.h>
#endif
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include <exception>
#include <iterator>
#include <memory>
#include <vector>

vtkStandardNewMacro(vtkCGALPatchFilling);

namespace pmp = CGAL::Polygon_mesh_processing;

//------------------------------------------------------------------------------
vtkCGALPatchFilling::vtkCGALPatchFilling()
{
  this->SetNumberOfInputPorts(2);
}

//------------------------------------------------------------------------------
void vtkCGALPatchFilling::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
void vtkCGALPatchFilling::SetUpdateAttributes(bool vtkNotUsed(update))
{
  vtkWarningMacro(
    "Unsupported: No attributes are interpolated with the vtkCGALPatchFilling filter.");
}

//------------------------------------------------------------------------------
int vtkCGALPatchFilling::FillInputPortInformation(int port, vtkInformation* info)
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
void vtkCGALPatchFilling::SetSourceConnection(vtkAlgorithmOutput* algOutput)
{
  this->SetInputConnection(1, algOutput);
}

//------------------------------------------------------------------------------
int vtkCGALPatchFilling::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  using Graph_halfedge = boost::graph_traits<CGAL_Surface>::halfedge_descriptor;

  // Get the input and output data objects.
  vtkPolyData* input  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  vtkNew<vtkPolyData> baseDataSet;
  baseDataSet->ShallowCopy(input);

  // result
  bool success = true;

  // Remove selected elements if any
  vtkInformation* selInfo = inputVector[1]->GetInformationObject(0);
  if (selInfo)
  {
    vtkSelection* inputSel = vtkSelection::SafeDownCast(selInfo->Get(vtkDataObject::DATA_OBJECT()));
    const auto    selNbNodes = inputSel->GetNumberOfNodes();
    if (selNbNodes > 0)
    {
      // pipeline to remove selection
      vtkNew<vtkExtractSelection> extractSelection;
      extractSelection->SetInputData(0, baseDataSet);
      extractSelection->SetInputData(1, inputSel);
      extractSelection->PreserveTopologyOn();
      vtkNew<vtkThreshold> threshold;
      threshold->SetInputConnection(extractSelection->GetOutputPort());
      threshold->SetInputArrayToProcess(
        0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS_THEN_CELLS, "vtkInsidedness");
      threshold->SetUpperThreshold(0.5);
      vtkNew<vtkDataSetSurfaceFilter> surface;
      surface->SetInputConnection(threshold->GetOutputPort());
      vtkNew<vtkTriangleFilter> tri;
      tri->SetInputConnection(surface->GetOutputPort());
      tri->Update();
      baseDataSet->ShallowCopy(vtkPolyData::SafeDownCast(tri->GetOutput(0)));
    }
  }

  // Create the triangle mesh for CGAL
  // --------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  if (vtkCGALHelper::toCGAL(baseDataSet, cgalMesh.get()) == false)
  {
    vtkErrorMacro("Non manifold input may lead to crash, please fix the input mesh.");
    return 0;
  }

  // CGAL Processing
  // ---------------

  std::vector<Graph_Verts> patch_vertices;
  std::vector<Graph_Faces> patch_facets;

  try
  {
    // collect one halfedge per boundary cycle
    std::vector<Graph_halfedge> borderCycles;
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(6,2,0)
    CGAL::extract_boundary_cycles(cgalMesh->surface, std::back_inserter(borderCycles));
#else
    pmp::extract_boundary_cycles(cgalMesh->surface, std::back_inserter(borderCycles));
#endif

    // fill boundary cycles
    for (Graph_halfedge h : borderCycles)
    {
      success &= std::get<0>(pmp::triangulate_refine_and_fair_hole(cgalMesh->surface, h,
        pmp::parameters::fairing_continuity(this->FairingContinuity)
          .face_output_iterator(std::back_inserter(patch_facets))
          .vertex_output_iterator(std::back_inserter(patch_vertices))));
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

  // Note, there is not UpdateAttributes here as the new mesh
  // contains patches not present in the initial surface

  return success;
}
