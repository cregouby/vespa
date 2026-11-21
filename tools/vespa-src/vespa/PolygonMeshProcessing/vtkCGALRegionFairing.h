/**
 * @class   vtkCGALRegionFairing
 * @brief   fair region using the CGAL fair method
 *
 * vtkCGALRegionFairing is a filter allowing fair blind-holes on
 * a triangulated polydata using the CGAL `fair` method.
 */

#ifndef vtkCGALRegionFairing_h
#define vtkCGALRegionFairing_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALPMPModule.h" // For export macro

class VTKCGALPMP_EXPORT vtkCGALRegionFairing : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALRegionFairing* New();
  vtkTypeMacro(vtkCGALRegionFairing, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Specify the selection describing the region to fair.
   */
  void SetSourceConnection(vtkAlgorithmOutput* algOutput);

protected:
  vtkCGALRegionFairing();
  ~vtkCGALRegionFairing() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;

private:
  vtkCGALRegionFairing(const vtkCGALRegionFairing&) = delete;
  void operator=(const vtkCGALRegionFairing&)       = delete;
};

#endif
