#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "vtkCGALAlgorithm" for configuration "Release"
set_property(TARGET vtkCGALAlgorithm APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCGALAlgorithm PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCGALAlgorithm.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libvtkCGALAlgorithm.dll"
  )

list(APPEND _cmake_import_check_targets vtkCGALAlgorithm )
list(APPEND _cmake_import_check_files_for_vtkCGALAlgorithm "${_IMPORT_PREFIX}/lib/libvtkCGALAlgorithm.dll.a" "${_IMPORT_PREFIX}/bin/libvtkCGALAlgorithm.dll" )

# Import target "vtkCGALPSP" for configuration "Release"
set_property(TARGET vtkCGALPSP APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCGALPSP PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCGALPSP.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libvtkCGALPSP.dll"
  )

list(APPEND _cmake_import_check_targets vtkCGALPSP )
list(APPEND _cmake_import_check_files_for_vtkCGALPSP "${_IMPORT_PREFIX}/lib/libvtkCGALPSP.dll.a" "${_IMPORT_PREFIX}/bin/libvtkCGALPSP.dll" )

# Import target "vtkCGALSR" for configuration "Release"
set_property(TARGET vtkCGALSR APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCGALSR PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCGALSR.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libvtkCGALSR.dll"
  )

list(APPEND _cmake_import_check_targets vtkCGALSR )
list(APPEND _cmake_import_check_files_for_vtkCGALSR "${_IMPORT_PREFIX}/lib/libvtkCGALSR.dll.a" "${_IMPORT_PREFIX}/bin/libvtkCGALSR.dll" )

# Import target "vtkCGALPMP" for configuration "Release"
set_property(TARGET vtkCGALPMP APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCGALPMP PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCGALPMP.dll.a"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "Ceres::ceres"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libvtkCGALPMP.dll"
  )

list(APPEND _cmake_import_check_targets vtkCGALPMP )
list(APPEND _cmake_import_check_files_for_vtkCGALPMP "${_IMPORT_PREFIX}/lib/libvtkCGALPMP.dll.a" "${_IMPORT_PREFIX}/bin/libvtkCGALPMP.dll" )

# Import target "vtkCGALDelaunay" for configuration "Release"
set_property(TARGET vtkCGALDelaunay APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(vtkCGALDelaunay PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libvtkCGALDelaunay.dll.a"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libvtkCGALDelaunay.dll"
  )

list(APPEND _cmake_import_check_targets vtkCGALDelaunay )
list(APPEND _cmake_import_check_files_for_vtkCGALDelaunay "${_IMPORT_PREFIX}/lib/libvtkCGALDelaunay.dll.a" "${_IMPORT_PREFIX}/bin/libvtkCGALDelaunay.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
