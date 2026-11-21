/**
 * @class   vtkCGALPatchFilling
 * @brief   fill patches and holes using CGAL
 *
 * vtkCGALPatchFilling is a filter allowing to fill holes in the
 * mesh and remesh patches on a triangulated polydata. It optionally
 * recieves a vtkSelection containing a list of triangles or points,
 * remove them from the mesh, then fill the corresponding holes with the
 * triangulate_refine_and_fair_hole method.  This filter may also be used to
 * fill tunnels by selection the inner cells. Contrary to the vtkCGALIsotropicRemesh,
 * it won't keep the initial shape.
 */

#ifndef vtkCGALPatchFilling_h
#define vtkCGALPatchFilling_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALPMPModule.h" // For export macro

class VTKCGALPMP_EXPORT vtkCGALPatchFilling : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALPatchFilling* New();
  vtkTypeMacro(vtkCGALPatchFilling, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void SetUpdateAttributes(bool update) override;

  /**
   * Specify the selection describing the hole or patch to fill.
   */
  void SetSourceConnection(vtkAlgorithmOutput* algOutput);

  ///@{
  /**
   * Drive the boundary tagencial continuity parameter
   * given to CGAL:
   * A value controling the tangential continuity of the output surface patch.
   * The possible values are 0, 1 and 2, refering to the C0, C1 and C2 continuity
   * Default is 1.
   * Use 0 for plannar filling.
   */
  vtkGetMacro(FairingContinuity, int);
  vtkSetClampMacro(FairingContinuity, int, 0, 2);
  ///@}

protected:
  vtkCGALPatchFilling();
  ~vtkCGALPatchFilling() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;

  int FairingContinuity = 1;

private:
  vtkCGALPatchFilling(const vtkCGALPatchFilling&) = delete;
  void operator=(const vtkCGALPatchFilling&)      = delete;
};

#endif
