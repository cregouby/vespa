/**
 * @class   vtkCGALMeshChecker
 * @brief   Check various properties of a mesh, like manifoldness
 *
 * vtkCGALMeshChecker is a filter allowing to perform diagnosis on a mesh
 * to check manifoldness, auto-intersection, watertightness...
 */

#ifndef vtkCGALMeshChecker_h
#define vtkCGALMeshChecker_h

#include "vtkCGALPolyDataAlgorithm.h"

#include "vtkCGALPMPModule.h" // For export macro

class VTKCGALPMP_EXPORT vtkCGALMeshChecker : public vtkCGALPolyDataAlgorithm
{
public:
  static vtkCGALMeshChecker* New();
  vtkTypeMacro(vtkCGALMeshChecker, vtkCGALPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  // Accessors:

  // {@
  /**
   *   Set / Get the CheckWatertight property, default: true
   *   If true, check the input mesh is closed and bounds a volume
   *   using the accordingly named CGAL methods.
   */
  vtkGetMacro(CheckWatertight, bool);
  vtkSetMacro(CheckWatertight, bool);
  vtkBooleanMacro(CheckWatertight, bool);
  // }@

  // {@
  /**
   *   Set / Get the CheckIntersect property, default: true
   *   If true, check the input mesh does not self intersect
   *   using the accordingly named CGAL methods.
   */
  vtkGetMacro(CheckIntersect, bool);
  vtkSetMacro(CheckIntersect, bool);
  vtkBooleanMacro(CheckIntersect, bool);
  // }@

  // {@
  /**
   *   Set / Get the AttemptRepair property, default: false
   *   If true, this filter will try to repair non conformal meshes
   *    by various ways.
   *   This triggers an additional deep copy.
   */
  vtkGetMacro(AttemptRepair, bool);
  vtkSetMacro(AttemptRepair, bool);
  vtkBooleanMacro(AttemptRepair, bool);
  // }@

protected:
  vtkCGALMeshChecker()           = default;
  ~vtkCGALMeshChecker() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  // fields
  bool CheckWatertight = true;
  bool CheckIntersect  = true;
  bool AttemptRepair   = false;

private:
  vtkCGALMeshChecker(const vtkCGALMeshChecker&) = delete;
  void operator=(const vtkCGALMeshChecker&)     = delete;
};

#endif
