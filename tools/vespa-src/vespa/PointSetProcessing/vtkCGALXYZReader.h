/**
 * @class   vtkCGALXYZReader
 * @brief   Reader for CGAL point cloud data.
 *
 * vtkCGALXYZReader reads point clouds (.las, .off, .ply, .xyz) using
 * CGAL point cloud data reader functionality.
 */

#ifndef vtkCGALXYZReader_h
#define vtkCGALXYZReader_h

#include "vtkCGALPSPModule.h" // For export macro
#include "vtkCGALPolyDataAlgorithm.h"

class vtkDataArraySelection;
class vtkDataSet;
class vtkDoubleArray;
class vtkIntArray;
class vtkStdString;
class vtkStringArray;
class vtkAIMToolsPrivate;

class VTKCGALPSP_EXPORT vtkCGALXYZReader : public vtkCGALPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkCGALXYZReader, vtkCGALPolyDataAlgorithm);
  static vtkCGALXYZReader* New();
  void                     PrintSelf(ostream& os, vtkIndent indent) override;

  vtkSetFilePathMacro(FileName);
  vtkGetFilePathMacro(FileName);

protected:
  vtkCGALXYZReader();
  ~vtkCGALXYZReader() override;

  int RequestData(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

private:
  vtkCGALXYZReader(const vtkCGALXYZReader&) = delete;
  void operator=(const vtkCGALXYZReader&)   = delete;

  char* FileName;
};

#endif // vtkCGALXYZReader_h
