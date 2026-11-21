#include <vtkABINamespace.h>

VTK_ABI_NAMESPACE_BEGIN
class vtkDIYUtilitiesCleanup
{
public:
  vtkDIYUtilitiesCleanup();
  ~vtkDIYUtilitiesCleanup();

private:
  vtkDIYUtilitiesCleanup(const vtkDIYUtilitiesCleanup&) = delete;
  void operator=(const vtkDIYUtilitiesCleanup&) = delete;
};

vtkDIYUtilitiesCleanup::vtkDIYUtilitiesCleanup() = default;
vtkDIYUtilitiesCleanup::~vtkDIYUtilitiesCleanup() = default;
VTK_ABI_NAMESPACE_END
