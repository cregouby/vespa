/**
 * @class   vtkCGALSignedDistanceFunction
 * @brief   generate signed distance field from polydata
 *
 * vtkCGALSignedDistanceFunction is a filter allowing to construct
 * a vtkImageData where each point is the shortest signed distance
 * to the input surface represented as a polydata. The input has to
 * be manifold to properly distinguished the inside from the outside
 * and triangulated.
 */

#ifndef vtkCGALSignedDistanceFunction_h
#define vtkCGALSignedDistanceFunction_h

#include "vtkCGALPMPModule.h" // For export macro

#include <vtkDataObjectAlgorithm.h>

class VTKCGALPMP_EXPORT vtkCGALSignedDistanceFunction : public vtkDataObjectAlgorithm
{
public:
  static vtkCGALSignedDistanceFunction* New();
  vtkTypeMacro(vtkCGALSignedDistanceFunction, vtkDataObjectAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Get / Set the BaseResolution property.
   * This actually defines the resolution on the smallest dimension. The 2
   * other resolutions are deduced to keep the sampling isotropic.
   * Default: 64
   **/
  vtkGetMacro(BaseResolution, unsigned int);
  vtkSetMacro(BaseResolution, unsigned int);
  //@}

  //@{
  /**
   * Get / Set the Padding property, controlling the number of sample layers to
   * add in each direction.
   * Default: 0
   **/
  vtkGetMacro(Padding, int);
  vtkSetMacro(Padding, int);
  //@}

protected:
  vtkCGALSignedDistanceFunction()           = default;
  ~vtkCGALSignedDistanceFunction() override = default;

  // VTK functions
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;
  int FillOutputPortInformation(int port, vtkInformation* info) override;

private:
  vtkCGALSignedDistanceFunction(const vtkCGALSignedDistanceFunction&) = delete;
  void operator=(const vtkCGALSignedDistanceFunction&)                = delete;

  int Padding        = 0;
  int BaseResolution = 64;
};

#endif
