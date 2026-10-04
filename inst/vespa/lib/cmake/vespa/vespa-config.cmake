
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was vespa.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)

set(VESPA_VTK_DIR "C:/rtools45/ucrt64/lib/cmake/vtk")
if(NOT VTK_FOUND)
  find_dependency(VTK PATHS "${VESPA_VTK_DIR}")
else()
  get_filename_component(tmp1 "${VESPA_VTK_DIR}" REALPATH)
  get_filename_component(tmp2 "${VTK_DIR}" REALPATH)
  if(NOT "${tmp1}" STREQUAL "${tmp2}")
    message(WARNING
      "Mismatch for VTK between vespa and current project: "
      "VTK_VESPA_DIR=${VESPA_VTK_DIR} "
      "VTK_DIR=${VTK_DIR}")
  endif()
endif()

set(VESPA_CGAL_DIR "C:/rtools45/ucrt64/lib/cmake/CGAL")
if(NOT CGAL_FOUND)
  find_dependency(CGAL PATHS "${VESPA_CGAL_DIR}")
else()
  get_filename_component(tmp1 "${VESPA_CGAL_DIR}" REALPATH)
  get_filename_component(tmp2 "${CGAL_DIR}" REALPATH)
  if(NOT "${tmp1}" STREQUAL "${tmp2}")
    message(WARNING
      "Mismatch for CGAL between vespa and current project: "
      "CGAL_VESPA_DIR=${VESPA_CGAL_DIR} "
      "CGAL_DIR=${CGAL_DIR}")
  endif()
endif()

set(VESPA_EIGEN3_DIR "C:/rtools45/ucrt64/share/eigen3/cmake")
if(NOT Eigen3_FOUND)
  find_dependency(Eigen3 PATHS "${VESPA_EIGEN3_DIR}")
else()
  get_filename_component(tmp1 "${VESPA_EIGEN3_DIR}" REALPATH)
  get_filename_component(tmp2 "${Eigen3_DIR}" REALPATH)
  if(NOT "${tmp1}" STREQUAL "${tmp2}")
    message(WARNING
      "Mismatch for Eigen3 between vespa and current project: "
      "VESPA_EIGEN3_DIR=${VESPA_EIGEN3_DIR} "
      "Eigen3_DIR=${Eigen3_DIR}")
  endif()
endif()

# The exported vtkCGALPMP and vtkCGALSR targets carry
# $<LINK_ONLY:CGAL::Eigen3_support> in their interface, so consumers must have
# that imported target defined. CGAL provides it through this module (found via
# the module path set up by find_dependency(CGAL) above).
include(CGAL_Eigen3_support)

include("${CMAKE_CURRENT_LIST_DIR}/vespa-targets.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/vespa-vtk-module-properties.cmake")
