set(vtkcgalpsp_files
  vtkCGALPCAEstimateNormals
  vtkCGALXYZReader
)

vtk_module_add_module(vtkCGALPSP
  ${FORCE_STATIC_MODULES_STRING}
  CLASSES ${vtkcgalpsp_files}
)
