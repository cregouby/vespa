#include "vtkCGALShapeSmoothing.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

// CGAL related includes
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>

#include <exception>
#include <memory>

vtkStandardNewMacro(vtkCGALShapeSmoothing);

namespace pmp = CGAL::Polygon_mesh_processing;

//------------------------------------------------------------------------------
void vtkCGALShapeSmoothing::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "Number of Iterations :" << this->NumberOfIterations << std::endl;
  os << indent << "Time Step :" << this->TimeStep << std::endl;
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int vtkCGALShapeSmoothing::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects
  vtkPolyData* input  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  if (!input || !output)
  {
    vtkErrorMacro("Missing input or output.");
  }

  // Create the surface mesh for CGAL
  // ----------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  vtkCGALHelper::toCGAL(input, cgalMesh.get());

  // CGAL Processing
  // ---------------

  try
  {
    pmp::smooth_shape(cgalMesh->surface, this->TimeStep,
      pmp::parameters::number_of_iterations(this->NumberOfIterations));
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
