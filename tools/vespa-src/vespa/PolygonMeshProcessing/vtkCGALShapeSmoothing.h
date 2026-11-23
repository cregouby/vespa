/**
 * @class   vtkCGALShapeSmoothing
 * @brief   Smoothes a surface mesh by moving its vertices.
 *
 * vtkCGALShapeSmoothing is a filter that smoothes the overall shape of a 3D mesh
 * using the mean curvature.
 * The degree of smoothing can be controlled using the number of iterations as well as
 * the time step. The time step specifies the smoothing speed.
 * A higher time step results in a stronger shape distortion than a higher number of iterations.
 */

#ifndef vtkCGALShapeSmoothing_h
#define vtkCGALShapeSmoothing_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALPMPModule.h" // For export macro

class VTKCGALPMP_EXPORT vtkCGALShapeSmoothing : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALShapeSmoothing* New();
  vtkTypeMacro(vtkCGALShapeSmoothing, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Get/set the number of iterations used in the smoothing process.
   * Default is 1.
   **/
  vtkGetMacro(NumberOfIterations, unsigned int);
  vtkSetMacro(NumberOfIterations, unsigned int);
  ///@}

  ///@{
  /**
   * Get/set the time step indicating the smoothing speed.
   * A higher value leads to a faster distortion of the shape.
   * Standard values lie in the range 1e-6 to 1.
   * Default is 1e-4.
   **/
  vtkGetMacro(TimeStep, double);
  vtkSetMacro(TimeStep, double);
  ///@}

protected:
  vtkCGALShapeSmoothing()           = default;
  ~vtkCGALShapeSmoothing() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  unsigned int NumberOfIterations = 1;
  double       TimeStep           = 1e-4;

private:
  vtkCGALShapeSmoothing(const vtkCGALShapeSmoothing&) = delete;
  void operator=(const vtkCGALShapeSmoothing&)        = delete;
};

#endif
