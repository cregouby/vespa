set(vtkcgalsr_files
  vtkCGALPoissonSurfaceReconstructionDelaunay
  vtkCGALAdvancingFrontSurfaceReconstruction
)
vtk_module_add_module(vtkCGALSR
  ${FORCE_STATIC_MODULES_STRING}
  CLASSES ${vtkcgalsr_files}
)
vtk_module_link(vtkCGALSR PRIVATE CGAL::Eigen3_support)
