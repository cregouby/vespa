/**
 * @class   vtkCGALMeshSubdivision
 * @brief   Refines a polygonal mesh through subdivision.
 *
 * This filter performs a surface mesh subdivision by creating new points
 * to refine and smoothen a polygonal mesh. Several subdivision methods are
 * available:
 *   - Catmull-Clark based on the PQQ pattern
 *   - Loop based on the PTQ pattern
 *   - Doo-Sabin based on the DQQ pattern
 *   - Sqrt3 based on the Sqrt3 pattern
 */

#ifndef vtkCGALMeshSubdivision_h
#define vtkCGALMeshSubdivision_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALPMPModule.h" // For export macro

class VTKCGALPMP_EXPORT vtkCGALMeshSubdivision : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALMeshSubdivision* New();
  vtkTypeMacro(vtkCGALMeshSubdivision, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * List of available subdivision methods, each based on a specific refinement pattern.
   **/
  enum SubdivisionMethod
  {
    CATMULL_CLARK = 0,
    LOOP,
    DOO_SABIN,
    SQRT3
  };

  ///@{
  /**
   * Get/set the subdivision method.
   * Default is SQRT3.
   **/
  vtkGetMacro(SubdivisionType, int);
  vtkSetClampMacro(
    SubdivisionType, int, vtkCGALMeshSubdivision::CATMULL_CLARK, vtkCGALMeshSubdivision::SQRT3);
  ///@}

  ///@{
  /**
   * Get/set the number of iterations (subdivisions) used in the subdivision process.
   * Default is 1.
   **/
  vtkGetMacro(NumberOfIterations, double);
  vtkSetMacro(NumberOfIterations, double);
  ///@}

protected:
  vtkCGALMeshSubdivision()           = default;
  ~vtkCGALMeshSubdivision() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  int          SubdivisionType    = vtkCGALMeshSubdivision::SQRT3;
  unsigned int NumberOfIterations = 1;

private:
  vtkCGALMeshSubdivision(const vtkCGALMeshSubdivision&) = delete;
  void operator=(const vtkCGALMeshSubdivision&)         = delete;
};

#endif
