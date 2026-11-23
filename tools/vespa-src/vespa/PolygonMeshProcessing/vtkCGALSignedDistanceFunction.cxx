#include "vtkCGALSignedDistanceFunction.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkDataSet.h>
#include <vtkFloatArray.h>
#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkSMPTools.h>

// CGAL related includes
#include <CGAL/version.h>

#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/AABB_tree.h>
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(6, 0, 0)
#include <CGAL/AABB_traits_3.h>
#else
#include <CGAL/AABB_traits.h>
#endif
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/Side_of_triangle_mesh.h>

// clang-format off
using Point                 = CGAL_Kernel::Point_3;
using Side_of_triangle_mesh = CGAL::Side_of_triangle_mesh<CGAL_Surface, CGAL_Kernel>;
using Primitive             = CGAL::AABB_face_graph_triangle_primitive<CGAL_Surface>;
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(6, 0, 0)
using Traits                = CGAL::AABB_traits_3<CGAL_Kernel, Primitive>;
#else
using Traits                = CGAL::AABB_traits<CGAL_Kernel, Primitive>;
#endif
using Tree                  = CGAL::AABB_tree<Traits>;
// clang-format on

vtkStandardNewMacro(vtkCGALSignedDistanceFunction);

//------------------------------------------------------------------------------
void vtkCGALSignedDistanceFunction::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Padding: " << this->Padding << std::endl;
  os << indent << "Base Resolution: " << this->BaseResolution << std::endl;
}

//------------------------------------------------------------------------------
int vtkCGALSignedDistanceFunction::FillInputPortInformation(
  int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
  return 1;
}

//------------------------------------------------------------------------------
int vtkCGALSignedDistanceFunction::FillOutputPortInformation(
  int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData");
  return 1;
}

//------------------------------------------------------------------------------
int vtkCGALSignedDistanceFunction::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  // Get the input and output data objects.
  vtkPolyData*  input  = vtkPolyData::GetData(inputVector[0]);
  vtkImageData* output = vtkImageData::GetData(outputVector);

  if (this->Padding <= -this->BaseResolution / 2)
  {
    vtkErrorMacro("Negative padding should be greater than negative half the base resolution.");
    return 0;
  }

  // Create the triangle mesh for CGAL
  // --------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalMesh =
    std::make_unique<vtkCGALHelper::Vespa_surface>();
  vtkCGALHelper::toCGAL(input, cgalMesh.get());

  if (!CGAL::is_closed(cgalMesh->surface))
  {
    vtkErrorMacro("The input mesh should be closed.");
    return 0;
  }
  if (!CGAL::is_triangle_mesh(cgalMesh->surface))
  {
    vtkErrorMacro("The input mesh should be triangulated.");
    return 0;
  }

  // CGAL Processing
  // ---------------

  vtkNew<vtkFloatArray> sdf;
  sdf->SetName("SDF");
  sdf->SetNumberOfComponents(1);

  try
  {
    Tree tree(faces(cgalMesh->surface).first, faces(cgalMesh->surface).second, cgalMesh->surface);
    tree.accelerate_distance_queries();
    Side_of_triangle_mesh insideTest(tree, CGAL_Kernel());

    CGAL::Bbox_3 bb   = CGAL::Polygon_mesh_processing::bbox(cgalMesh->surface);
    double       xMin = bb.xmin(), xMax = bb.xmax();
    double       yMin = bb.ymin(), yMax = bb.ymax();
    double       zMin = bb.zmin(), zMax = bb.zmax();

    float Lx        = xMax - xMin;
    float Ly        = yMax - yMin;
    float Lz        = zMax - zMin;
    float Lmin      = std::min({ Lx, Ly, Lz });
    float voxelSize = Lmin / (this->BaseResolution - 1);

    int Nx = int(std::round(Lx / voxelSize)) + 1 + 2 * this->Padding;
    int Ny = int(std::round(Ly / voxelSize)) + 1 + 2 * this->Padding;
    int Nz = int(std::round(Lz / voxelSize)) + 1 + 2 * this->Padding;

    const float dx = (xMax - xMin) / (Nx - 2 * this->Padding - 1);
    const float dy = (yMax - yMin) / (Ny - 2 * this->Padding - 1);
    const float dz = (zMax - zMin) / (Nz - 2 * this->Padding - 1);

    sdf->SetNumberOfTuples(Nx * Ny * Nz);
    output->SetDimensions(Nx, Ny, Nz);
    output->SetSpacing(dx, dy, dz);
    output->SetOrigin(xMin, yMin, zMin);

    auto idx = [&](int i, int j, int k) { return i + Nx * (j + Ny * k); };

    vtkSMPTools::For(0, Nz,
      [&](vtkIdType begin, vtkIdType end)
      {
        for (int k = begin; k < end; ++k)
        {
          float z = zMin + (k - this->Padding) * dz;
          for (int j = 0; j < Ny; ++j)
          {
            float y = yMin + (j - this->Padding) * dy;
            for (int i = 0; i < Nx; ++i)
            {
              float x = xMin + (i - this->Padding) * dx;

              Point sampledPoint(x, y, z);
              float distance = std::sqrt(tree.squared_distance(sampledPoint));
              float signedDistance =
                (insideTest(sampledPoint) == CGAL::ON_BOUNDED_SIDE) ? -distance : distance;
              sdf->SetTuple1(idx(i, j, k), signedDistance);
            }
          }
        }
      });
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // VTK Output
  // ----------
  output->GetPointData()->SetScalars(sdf);

  return 1;
}
