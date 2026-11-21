/**
 * @class   vtkCGALMeshDeformation
 * @brief   Deforms a surface mesh
 *
 * vtkCGALMeshDeformation is a filter that deforms a surface mesh by moving
 * control points to target positions. Neighboring points contained in a
 * Region of Interest (ROI) may be moved to obtain a smooth deformation.
 *
 * The filter can take three inputs (on ports 0, 1, 2, respectively):
 *   - the vtkPolyData mesh to deform with unique IDs for the points
 *   - a vtkPointSet with the target positions of the control points, identified by their IDs
 *   - a vtkSelection corresponding to the ROI (optional)
 *
 * If a ROI is not specified, it is defined with the control points.
 * In this case, the control points will simply be moved to their destinations without
 * modifying the rest of the mesh.
 */

#ifndef vtkCGALMeshDeformation_h
#define vtkCGALMeshDeformation_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALPMPModule.h" // For export macro

class VTKCGALPMP_EXPORT vtkCGALMeshDeformation : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALMeshDeformation* New();
  vtkTypeMacro(vtkCGALMeshDeformation, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Available modes for the deformation. Smooth tends to minimize non-linear
   * deformation, while Smoothed Rotation Enhanced As Rigid As Possible (SRE_ARAP) tends to keep the
   * initial shape
   */
  enum Mode
  {
    SMOOTH = 0,
    SRE_ARAP
  };

  /**
   * Set input connection for the second input (vtkPointSet).
   **/
  void SetSourceConnection(vtkAlgorithmOutput* algOutput);

  /**
   * Set input connection for the third input (vtkSelection).
   **/
  void SetSelectionConnection(vtkAlgorithmOutput* algOutput);

  ///@{
  /**
   * Get set the deformation mode,
   * default is SMOOTH
   */
  vtkGetMacro(Mode, int);
  vtkSetClampMacro(Mode, int, vtkCGALMeshDeformation::SMOOTH, vtkCGALMeshDeformation::SRE_ARAP);
  ///@}

  ///@{
  /**
   * Get/set the rigidity to use for angle deformation in SRE_ARAP mode.
   * Default is 0.02.
   **/
  vtkGetMacro(SreAlpha, double);
  vtkSetMacro(SreAlpha, double);
  ///@}

  ///@{
  /**
   * Get/set the number of iterations used in the deformation process.
   * Default is 5.
   **/
  vtkGetMacro(NumberOfIterations, unsigned int);
  vtkSetMacro(NumberOfIterations, unsigned int);
  ///@}

  ///@{
  /**
   * Get/set the tolerance of the energy convergence used in the deformation process.
   * Default is 1e-4.
   **/
  vtkGetMacro(Tolerance, double);
  vtkSetMacro(Tolerance, double);
  ///@}

  ///@{
  /**
   * Get/set the name of the array containing the IDs to use when defining ROI and control points.
   * Default is the array returned by GetGlobalIds for the points.
   **/
  vtkGetMacro(GlobalIdArray, std::string);
  vtkSetMacro(GlobalIdArray, std::string);
  ///@}

protected:
  vtkCGALMeshDeformation();
  ~vtkCGALMeshDeformation() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;

  int          Mode               = vtkCGALMeshDeformation::SMOOTH;
  double       SreAlpha           = 0.02;
  unsigned int NumberOfIterations = 5;
  double       Tolerance          = 1e-4;
  std::string  GlobalIdArray      = "";

private:
  vtkCGALMeshDeformation(const vtkCGALMeshDeformation&) = delete;
  void operator=(const vtkCGALMeshDeformation&)         = delete;
};

#endif
