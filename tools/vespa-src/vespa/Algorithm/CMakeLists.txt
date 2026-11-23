set(vtkcgalalgorithm_files
  vtkCGALPolyDataAlgorithm
  vtkCGALHelper
)

set(headers
  vtkCGALHelper.h)

set(sources
  vtkCGALHelper.cxx)

vtk_module_add_module(vtkCGALAlgorithm
  ${FORCE_STATIC_MODULES_STRING}
  CLASSES ${vtkcgalalgorithm_files}
  HEADERS ${headers}
  SOURCES ${sources}
)
