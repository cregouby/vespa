#include "vtkCGALXYZReader.h"

// VTK related includes
#include <vtkDoubleArray.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPointData.h>

// CGAL related includes
#include <CGAL/IO/read_points.h>

#include <exception>
#include <iterator>
#include <utility>
#include <vector>

typedef CGAL_Kernel::Point_3                                 Point;
typedef CGAL_Kernel::Vector_3                                Vector;
typedef std::pair<Point, Vector>                             Point_with_normal;
typedef CGAL::First_of_pair_property_map<Point_with_normal>  Point_map;
typedef CGAL::Second_of_pair_property_map<Point_with_normal> Normal_map;
typedef std::vector<Point_with_normal>                       PointList;

//=============================================================================
vtkStandardNewMacro(vtkCGALXYZReader);

//------------------------------------------------------------------------------
vtkCGALXYZReader::vtkCGALXYZReader()
{
  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(1);

  this->FileName = nullptr;
}

//------------------------------------------------------------------------------
vtkCGALXYZReader::~vtkCGALXYZReader()
{
  this->SetFileName(nullptr);
}

//------------------------------------------------------------------------------
void vtkCGALXYZReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "FileName: " << (this->FileName ? this->FileName : "(nullptr)") << endl;
}

//------------------------------------------------------------------------------
int vtkCGALXYZReader::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** vtkNotUsed(inputVector), vtkInformationVector* outputVector)
{
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  vtkPolyData*    output  = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  if (!output)
  {
    vtkErrorMacro(<< "Bad output type.");
    return 0;
  }

  PointList points;

  try
  {
    if (!CGAL::IO::read_points(this->FileName, std::back_inserter(points),
          CGAL::parameters::point_map(Point_map()).normal_map(Normal_map())))
    {
      vtkErrorMacro("Cannot read input file.");
      return 0;
    }
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception: " << e.what());
    return 0;
  }

  // VTK Output
  // ----------

  vtkNew<vtkPolyData> polydata;
  vtkNew<vtkPoints>   outpoints;

  vtkNew<vtkDoubleArray> pointNormalsArray;
  pointNormalsArray->SetName("Normals");
  pointNormalsArray->SetNumberOfComponents(3); // 3d normals (ie x,y,z)

  for (const auto& p : points)
  {
    outpoints->InsertNextPoint(p.first[0], p.first[1], p.first[2]);
    pointNormalsArray->InsertNextTuple3(p.second[0], p.second[1], p.second[2]);
  }

  polydata->SetPoints(outpoints);
  polydata->GetPointData()->AddArray(pointNormalsArray);
  polydata->GetPointData()->SetNormals(pointNormalsArray);

  output->DeepCopy(polydata);

  return 1;
}
