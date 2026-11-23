/**
 * @class   vtkCGALAdvancingFrontSurfaceReconstruction
 * @brief   Greedy algorithm for surface reconstruction from an unorganized point set
 *
 * vtkCGALAdvancingFrontSurfaceReconstruction implements a surface-based Delaunay surface
 reconstruction algorithm that sequentially selects the triangles, that is it uses previously
 selected triangles to select a new triangle for advancing the front. At each advancing step the
 most plausible triangle is selected, and such that the triangles selected generates an orientable
        manifold triangulated surface.
        Adapted from
        https://doc.cgal.org/latest/Advancing_front_surface_reconstruction/index.html#AFSR_Example_function
 */

#ifndef vtkCGALAdvancingFrontSurfaceReconstruction_h
#define vtkCGALAdvancingFrontSurfaceReconstruction_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALSRModule.h" // For export macro

class VTKCGALSR_EXPORT vtkCGALAdvancingFrontSurfaceReconstruction : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALAdvancingFrontSurfaceReconstruction* New();
  vtkTypeMacro(vtkCGALAdvancingFrontSurfaceReconstruction, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Get/set the perimeter bound.
   * Default is 0.0.
   **/
  vtkGetMacro(Per, double);
  vtkSetMacro(Per, double);

  /**
   * Get/set the radius ratio bound.
   * Default is 5.0.
   **/
  vtkGetMacro(RadiusRatioBound, double);
  vtkSetMacro(RadiusRatioBound, double);

protected:
  vtkCGALAdvancingFrontSurfaceReconstruction();
  ~vtkCGALAdvancingFrontSurfaceReconstruction() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkCGALAdvancingFrontSurfaceReconstruction(
    const vtkCGALAdvancingFrontSurfaceReconstruction&)              = delete;
  void operator=(const vtkCGALAdvancingFrontSurfaceReconstruction&) = delete;

  double Per;
  double RadiusRatioBound;
};

#endif
