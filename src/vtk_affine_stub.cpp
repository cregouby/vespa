// Explicit specializations for vtkImplicitArray<vtkAffineImplicitBackend<T>, 11>::SetValue
// that VTK 9.7 declares via extern template but does not export from its homebrew dylib
// (compiled with -fvisibility=hidden). Without these definitions, dlopen of vespa.so fails
// with "symbol not found in flat namespace" on macOS.
//
// SetValue is intentionally a no-op on implicit (read-only) arrays; the body matches
// the upstream implementation in vtkImplicitArray.txx.

// Suppress the extern template blocks before including the VTK headers so that
// we can provide our own explicit instantiation definitions below.
#define VTK_AFFINE_BACKEND_TEMPLATE_EXTERN
#define VTK_AFFINE_ARRAY_TEMPLATE_EXTERN

#include <vtkAffineImplicitBackend.h>
#include <vtkImplicitArray.h>

template void vtkImplicitArray<vtkAffineImplicitBackend<int>, 11>::SetValue(vtkIdType, int);
template void vtkImplicitArray<vtkAffineImplicitBackend<long long>, 11>::SetValue(vtkIdType, long long);
