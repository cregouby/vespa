/**
 * @class   vtkCGALBooleanOperation
 * @brief   Performs a Boolean operation between two vtkPolyData objects
 *
 * vtkCGALBooleanOperation is a filter allowing to perform a Boolean operation
 * between two closed, triangulated polygonal meshes.
 * These operations include union, intersection, and difference.
 * The resulting mesh is closed.
 */

#ifndef vtkCGALBooleanOperation_h
#define vtkCGALBooleanOperation_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALPMPModule.h" // For export macro

class VTKCGALPMP_EXPORT vtkCGALBooleanOperation : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALBooleanOperation* New();
  vtkTypeMacro(vtkCGALBooleanOperation, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * List of available Boolean operations between the volume bounded by the input
   * and the volume bounded by the source. The result is a closed mesh.
   **/
  enum Operation
  {
    DIFFERENCE = 0,
    INTERSECTION,
    UNION
  };

  ///@{
  /**
   * Get/set the type of Boolean operation.
   * Default is DIFFERENCE.
   **/
  vtkGetMacro(OperationType, int);
  vtkSetClampMacro(
    OperationType, int, vtkCGALBooleanOperation::DIFFERENCE, vtkCGALBooleanOperation::UNION);
  ///@}

  /**
   * Set input connection for the second vtkPolyData.
   **/
  void SetSourceConnection(vtkAlgorithmOutput* algOutput);

protected:
  vtkCGALBooleanOperation();
  ~vtkCGALBooleanOperation() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  int OperationType = vtkCGALBooleanOperation::DIFFERENCE;

private:
  vtkCGALBooleanOperation(const vtkCGALBooleanOperation&) = delete;
  void operator=(const vtkCGALBooleanOperation&)          = delete;
};

#endif
