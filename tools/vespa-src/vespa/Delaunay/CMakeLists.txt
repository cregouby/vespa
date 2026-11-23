set(vtkcgaldelaunay_files
  vtkCGALDelaunay2
)
vtk_module_add_module(vtkCGALDelaunay
  ${FORCE_STATIC_MODULES_STRING}
  CLASSES ${vtkcgaldelaunay_files}
)
