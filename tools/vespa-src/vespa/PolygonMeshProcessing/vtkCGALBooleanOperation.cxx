#include "vtkCGALBooleanOperation.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

// CGAL related includes
#include <CGAL/Polygon_mesh_processing/corefinement.h>

#include <exception>
#include <iostream>
#include <memory>

vtkStandardNewMacro(vtkCGALBooleanOperation);

namespace pmp = CGAL::Polygon_mesh_processing;

//------------------------------------------------------------------------------
vtkCGALBooleanOperation::vtkCGALBooleanOperation()
{
  this->SetNumberOfInputPorts(2);
}

//------------------------------------------------------------------------------
void vtkCGALBooleanOperation::PrintSelf(ostream& os, vtkIndent indent)
{
  switch (this->OperationType)
  {
    case vtkCGALBooleanOperation::DIFFERENCE:
      os << indent << "OperationType: " << "Difference" << std::endl;
      break;
    case vtkCGALBooleanOperation::INTERSECTION:
      os << indent << "OperationType: " << "Intersection" << std::endl;
      break;
    case vtkCGALBooleanOperation::UNION:
      os << indent << "OperationType: " << "Union" << std::endl;
      break;
    default:
      os << indent << "OperationType: " << "Unknown" << std::endl;
      break;
  }

  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
void vtkCGALBooleanOperation::SetSourceConnection(vtkAlgorithmOutput* algOutput)
{
  this->SetInputConnection(1, algOutput);
}

//------------------------------------------------------------------------------
int vtkCGALBooleanOperation::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and source data object
  vtkPolyData* inputData  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* sourceData = vtkPolyData::GetData(inputVector[1]);
  vtkPolyData* output     = vtkPolyData::GetData(outputVector);

  if (!inputData || !sourceData)
  {
    vtkErrorMacro("Missing input or source.");
  }

  // Create the surface meshes for CGAL
  // ----------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalInputMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  vtkCGALHelper::toCGAL(inputData, cgalInputMesh.get());
  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalSourceMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  vtkCGALHelper::toCGAL(sourceData, cgalSourceMesh.get());

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalOutMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();

  // CGAL Processing
  // ---------------

  bool res = true;
  try
  {
    if (!CGAL::is_closed(cgalInputMesh->surface))
    {
      vtkErrorMacro("Input Mesh is not closed; Boolean operation cannot be performed");
      return 0;
    }
    if (!CGAL::is_closed(cgalSourceMesh->surface))
    {
      vtkErrorMacro("Source Mesh is not closed; Boolean operation cannot be performed.");
      return 0;
    }

    // Preprocess
    if (!CGAL::Polygon_mesh_processing::does_bound_a_volume(cgalInputMesh->surface))
    {
      pmp::orient_to_bound_a_volume(cgalInputMesh->surface);
    }
    if (!CGAL::Polygon_mesh_processing::does_bound_a_volume(cgalSourceMesh->surface))
    {
      pmp::orient_to_bound_a_volume(cgalSourceMesh->surface);
    }

    // Main process
    try
    {
      switch (this->OperationType)
      {
        case vtkCGALBooleanOperation::DIFFERENCE:
          res = pmp::corefine_and_compute_difference(cgalInputMesh->surface,
            cgalSourceMesh->surface, cgalOutMesh->surface,
            pmp::parameters::throw_on_self_intersection(true), pmp::parameters::all_default());
          break;
        case vtkCGALBooleanOperation::INTERSECTION:
          res = pmp::corefine_and_compute_intersection(cgalInputMesh->surface,
            cgalSourceMesh->surface, cgalOutMesh->surface,
            pmp::parameters::throw_on_self_intersection(true), pmp::parameters::all_default());
          break;
        case vtkCGALBooleanOperation::UNION:
          res = pmp::corefine_and_compute_union(cgalInputMesh->surface, cgalSourceMesh->surface,
            cgalOutMesh->surface, pmp::parameters::throw_on_self_intersection(true),
            pmp::parameters::all_default());
          break;
        default:
          vtkErrorMacro("Unknown Boolean operation!");
          res = false;
          break;
      }
    }
    catch (const CGAL::Polygon_mesh_processing::Corefinement::Self_intersection_exception& e)
    {
      vtkErrorMacro("Self-intersections in meshes; Boolean operation cannot be performed.");
      return 0;
    }
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  if (!res)
  {
    // TODO: use the mesh checker instead here.

    // help user know the issue with their data.
    // Most of these checks are done after the processing for performance reasons
    std::cerr << "Boolean operation failed. Checking preconditions:" << std::endl;
    bool se1 = CGAL::Polygon_mesh_processing::does_self_intersect(cgalInputMesh->surface);
    bool se2 = CGAL::Polygon_mesh_processing::does_self_intersect(cgalSourceMesh->surface);
    std::cerr << "Input self intersect: " << se1 << std::endl;
    std::cerr << "Source self intersect: " << se2 << std::endl;
    bool bv1 = CGAL::Polygon_mesh_processing::does_bound_a_volume(cgalInputMesh->surface);
    bool bv2 = CGAL::Polygon_mesh_processing::does_bound_a_volume(cgalSourceMesh->surface);
    std::cerr << "Input bounds a volume: " << bv1 << std::endl;
    std::cerr << "Source bounds a volume: " << bv2 << std::endl;

    return 0;
  }

  // VTK Output
  // ----------

  vtkCGALHelper::toVTK(cgalOutMesh.get(), output);
  this->interpolateAttributes(inputData, output);

  return 1;
}
