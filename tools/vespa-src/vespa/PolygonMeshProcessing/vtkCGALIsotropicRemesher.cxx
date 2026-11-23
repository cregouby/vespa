#include "vtkCGALIsotropicRemesher.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

// CGAL related includes
#include <CGAL/Polygon_mesh_processing/detect_features.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>

#include <exception>
#include <memory>

vtkStandardNewMacro(vtkCGALIsotropicRemesher);

namespace pmp = CGAL::Polygon_mesh_processing;

//------------------------------------------------------------------------------
void vtkCGALIsotropicRemesher::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "TargetLength :" << this->TargetLength << std::endl;
  os << indent << "Number of Iterations :" << this->NumberOfIterations << std::endl;
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int vtkCGALIsotropicRemesher::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects.
  vtkPolyData* input  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  // edge target length
  auto targetLength = this->TargetLength;
  if (targetLength == -1)
  {
    // not specified by the user
    targetLength = 0.01 * input->GetLength();
  }
  if (targetLength <= 0)
  {
    vtkErrorMacro(
      "Please, specify a valid TargetLength for edges, current is: " << this->TargetLength);
    return 0;
  }

  // Create the surface mesh for CGAL
  // --------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  vtkCGALHelper::toCGAL(input, cgalMesh.get());

  // CGAL Processing
  // ---------------

  try
  {
    // protect feature edges:
    // https://doc.cgal.org/latest/Polygon_mesh_processing/Polygon_mesh_processing_2mesh_smoothing_example_8cpp-example.html#a3
    auto featureEdges = get(CGAL::edge_is_feature, cgalMesh->surface);
    pmp::detect_sharp_edges(cgalMesh->surface, this->ProtectAngle, featureEdges);

    // remesh
    pmp::isotropic_remeshing(cgalMesh->surface.faces(), targetLength, cgalMesh->surface,
      pmp::parameters::number_of_iterations(this->NumberOfIterations)
        .protect_constraints(true)
        .edge_is_constrained_map(featureEdges));
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
