#include "vtkCGALMeshSubdivision.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkTriangleFilter.h>

// CGAL related includes
#include <CGAL/subdivision_method_3.h>

#include <exception>
#include <memory>

vtkStandardNewMacro(vtkCGALMeshSubdivision);

//------------------------------------------------------------------------------
void vtkCGALMeshSubdivision::PrintSelf(ostream& os, vtkIndent indent)
{
  switch (this->SubdivisionType)
  {
    case vtkCGALMeshSubdivision::CATMULL_CLARK:
      os << indent << "SubdivisionType: " << "Catmull-Clark" << std::endl;
      break;
    case vtkCGALMeshSubdivision::LOOP:
      os << indent << "SubdivisionType: " << "Loop" << std::endl;
      break;
    case vtkCGALMeshSubdivision::DOO_SABIN:
      os << indent << "SubdivisionType: " << "Doo-Sabin" << std::endl;
      break;
    case vtkCGALMeshSubdivision::SQRT3:
      os << indent << "SubdivisionType: " << "Sqrt3" << std::endl;
      break;
    default:
      os << indent << "SubdivisionType: " << "Unknown" << std::endl;
      break;
  }

  os << indent << "Number of Iterations :" << this->NumberOfIterations << std::endl;
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int vtkCGALMeshSubdivision::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects.
  vtkPolyData* input  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  if (!input || !output)
  {
    vtkErrorMacro(<< "Missing input or output!");
    return 0;
  }

  // Create the triangle mesh for CGAL
  // ---------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  vtkCGALHelper::toCGAL(input, cgalMesh.get());

  // CGAL Processing
  // ---------------

  try
  {
    switch (this->SubdivisionType)
    {
      case vtkCGALMeshSubdivision::CATMULL_CLARK:
        CGAL::Subdivision_method_3::CatmullClark_subdivision(
          cgalMesh->surface, CGAL::parameters::number_of_iterations(this->NumberOfIterations));
        break;
      case vtkCGALMeshSubdivision::LOOP:
        CGAL::Subdivision_method_3::Loop_subdivision(
          cgalMesh->surface, CGAL::parameters::number_of_iterations(this->NumberOfIterations));
        break;
      case vtkCGALMeshSubdivision::DOO_SABIN:
        CGAL::Subdivision_method_3::DooSabin_subdivision(
          cgalMesh->surface, CGAL::parameters::number_of_iterations(this->NumberOfIterations));
        break;
      case vtkCGALMeshSubdivision::SQRT3:
        CGAL::Subdivision_method_3::Sqrt3_subdivision(
          cgalMesh->surface, CGAL::parameters::number_of_iterations(this->NumberOfIterations));
        break;
      default:
        vtkErrorMacro("Unknown subdivision method!");
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

  // Triangulate if needed
  if (this->SubdivisionType == vtkCGALMeshSubdivision::CATMULL_CLARK ||
    this->SubdivisionType == vtkCGALMeshSubdivision::DOO_SABIN)
  {
    vtkNew<vtkTriangleFilter> triangulator;
    triangulator->SetInputData(output);
    triangulator->Update();
    output->ShallowCopy(triangulator->GetOutput());
  }

  this->interpolateAttributes(input, output);

  return 1;
}
