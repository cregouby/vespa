/**
 * @class   vtkCGALPCAEstimateNormals
 * @brief   Normal estimation from an unorganized point set.
 *
 * vtkCGALPCAEstimateNormals estimates the normals of a point set through PCA (either over a fixed
        number of neighbors or using a spherical neighborhood radius of a factor times
        the average spacing) and orients the normals.
 * Adapted from
 * https://doc.cgal.org/latest/Point_set_processing_3/index.html#Point_set_processing_3NormalEstimation
 */

#ifndef vtkCGALPCAEstimateNormals_h
#define vtkCGALPCAEstimateNormals_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALPSPModule.h" // For export macro

class VTKCGALPSP_EXPORT vtkCGALPCAEstimateNormals : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALPCAEstimateNormals* New();
  vtkTypeMacro(vtkCGALPCAEstimateNormals, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Get/set the number of neighbors for computing the normal when Neighborhood=="Fixed Number of
   Neighbors" and for computing the average spacing when Neighborhood=="Fixed Radius".
   * Default is 18.
   **/
  vtkGetMacro(NumberOfNeighbors, unsigned int);
  vtkSetMacro(NumberOfNeighbors, unsigned int);

  /**
   * Get/set if normals will be oriented.
   * Default is true.
   **/
  vtkGetMacro(OrientNormals, bool);
  vtkSetMacro(OrientNormals, bool);

  /**
   * Get/set if unoriented normals wll be deleted.
   * Default is true.
   **/
  vtkGetMacro(DeleteUnoriented, bool);
  vtkSetMacro(DeleteUnoriented, bool);

protected:
  vtkCGALPCAEstimateNormals();
  ~vtkCGALPCAEstimateNormals() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

private:
  vtkCGALPCAEstimateNormals(const vtkCGALPCAEstimateNormals&) = delete;
  void operator=(const vtkCGALPCAEstimateNormals&)            = delete;

  unsigned int NumberOfNeighbors;
  bool         OrientNormals;
  bool         DeleteUnoriented;
};

#endif
