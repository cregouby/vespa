#include "vtkCGALAlphaWrapping.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

// CGAL related includes
#include <CGAL/alpha_wrap_3.h>

#include <exception>
#include <iostream>
#include <memory>

vtkStandardNewMacro(vtkCGALAlphaWrapping);

//------------------------------------------------------------------------------
void vtkCGALAlphaWrapping::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "Alpha :" << this->Alpha << std::endl;
  os << indent << "Offset :" << this->Offset << std::endl;
  this->Superclass::PrintSelf(os, indent);
}

//------------------------------------------------------------------------------
int vtkCGALAlphaWrapping::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects.
  vtkPolyData* input  = vtkPolyData::GetData(inputVector[0]);
  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  // input parameters

  if (this->Alpha <= 0)
  {
    vtkErrorMacro("Please, specify a positive Alpha: " << this->Alpha);
    return 0;
  }
  auto alpha = this->Alpha;

  if (this->Offset <= 0)
  {
    vtkErrorMacro("Please, specify a positive Offset: " << this->Offset);
    return 0;
  }
  auto offset = this->Offset;

  if (!this->AbsoluteThresholds)
  {
    auto length = input->GetLength();
    alpha *= length / 100.0;
    offset *= length / 100.0;
  }

  // Create the surface mesh for CGAL
  // --------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_soup> cgalMesh =
    std::make_unique<vtkCGALHelper::Vespa_soup>();
  vtkCGALHelper::toCGAL(input, cgalMesh.get());

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalOutput =
    std::make_unique<vtkCGALHelper::Vespa_surface>();

  // CGAL Processing
  // ---------------

  try
  {
    bool isPointCloud = true;
    for (auto& cell : cgalMesh->faces)
    {
      if (cell.size() > 1)
      {
        isPointCloud = false;
      }
    }

    if (isPointCloud)
    {
      // Specific version when we have only points
      CGAL::alpha_wrap_3(cgalMesh->points, alpha, offset, cgalOutput->surface);
    }
    else
    {
      CGAL::alpha_wrap_3(cgalMesh->points, cgalMesh->faces, alpha, offset, cgalOutput->surface);
    }
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // VTK Output
  // ----------

  vtkCGALHelper::toVTK(cgalOutput.get(), output);
  this->interpolateAttributes(input, output);

  return 1;
}
