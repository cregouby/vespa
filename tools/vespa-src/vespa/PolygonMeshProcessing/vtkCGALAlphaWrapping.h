/**
 * @class   vtkCGALAlphaWrapping
 * @brief   generate meshes from point clouds or triangle soups, using the CGAL Alpha wrapping.
 *
 * vtkCGALAlphaWrapping is a filter allowing to construct
 * a 2-manifold surface mesh from a point cloud or a polygon soup.
 * The resulting mesh is guaranteed to strictly encloses the input.
 * For now, only points and triangles are supported in input.
 */

#ifndef vtkCGALAlphaWrapping_h
#define vtkCGALAlphaWrapping_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALPMPModule.h" // For export macro

class VTKCGALPMP_EXPORT vtkCGALAlphaWrapping : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALAlphaWrapping* New();
  vtkTypeMacro(vtkCGALAlphaWrapping, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Get / Set the AbsoluteThresholds mode.
   * When disabled, Alpha and Offset are given
   * as percentage of the diagonal length.
   **/
  vtkGetMacro(AbsoluteThresholds, bool);
  vtkSetMacro(AbsoluteThresholds, bool);
  vtkBooleanMacro(AbsoluteThresholds, bool);
  //@}

  //@{
  /**
   * Get / Set the Alpha property,
   * controlling the maximum circumcenter of the faces in the resulting mesh.
   * A smaller Alpha means a better penetration into concave parts of the mesh.
   **/
  vtkGetMacro(Alpha, double);
  vtkSetMacro(Alpha, double);
  //@}

  //@{
  /**
   * Get / Set the Offset property, controlling the dilatation
   * of the resulting mesh with respect to the input mesh.
   * The dilatation value must be non-null and garantees a 2-manifold output.
   **/
  vtkGetMacro(Offset, double);
  vtkSetMacro(Offset, double);
  //@}

protected:
  vtkCGALAlphaWrapping()           = default;
  ~vtkCGALAlphaWrapping() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  // Fields

  bool   AbsoluteThresholds = false;
  double Alpha              = 5;
  double Offset             = 3;

private:
  vtkCGALAlphaWrapping(const vtkCGALAlphaWrapping&) = delete;
  void operator=(const vtkCGALAlphaWrapping&)       = delete;
};

#endif
