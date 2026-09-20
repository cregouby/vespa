cmake_minimum_required(VERSION 3.19)
project(VespaBuild NONE)

include(ExternalProject)

# Installation prefix for Vespa build artifacts.
# When invoked from the configure script, this is set to <pkg_source>/inst/vespa.
set(INSTALL_PREFIX "${CMAKE_BINARY_DIR}/vespa-install"
    CACHE PATH "Destination directory for Vespa headers and libraries")

# Git tag or commit read from .github/vespa-version.txt and forwarded by configure.
set(VESPA_GIT_TAG "v1.0"
    CACHE STRING "Vespa git tag or commit hash to build")

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Detect whether the tag looks like a raw hash (shallow clone only works with named refs).
string(LENGTH "${VESPA_GIT_TAG}" _tag_len)
if(_tag_len GREATER 15 AND "${VESPA_GIT_TAG}" MATCHES "^[0-9a-f]+$")
  set(_git_shallow FALSE)
else()
  set(_git_shallow TRUE)
endif()

# Forward the compilers chosen by R's toolchain so the ABI matches.
if(DEFINED ENV{CC})
  set(_CC_ARG  "-DCMAKE_C_COMPILER=$ENV{CC}")
endif()
if(DEFINED ENV{CXX})
  set(_CXX_ARG "-DCMAKE_CXX_COMPILER=$ENV{CXX}")
endif()

ExternalProject_Add(vespa
  GIT_REPOSITORY "https://gitlab.kitware.com/vtk/meshing/vespa.git"
  GIT_TAG        "${VESPA_GIT_TAG}"
  GIT_SHALLOW    ${_git_shallow}
  GIT_PROGRESS   TRUE

  # Eigen 5 (Homebrew 2025+) is API-compatible with 3.x but its cmake config
  # rejects find_package(Eigen3 3.2.0). Widen the version range so both work.
  PATCH_COMMAND
    sed -i.bak
      -e "s|find_package(Eigen3 3\\.2\\.0 REQUIRED|find_package(Eigen3 REQUIRED|g"
      -e "s|find_package(CGAL 5\\.3\\.0 REQUIRED|find_package(CGAL REQUIRED|g"
      <SOURCE_DIR>/CMakeLists.txt &&
    ${CMAKE_COMMAND} -E rm -f <SOURCE_DIR>/CMakeLists.txt.bak
    
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
    ${_CC_ARG}
    ${_CXX_ARG}

  # Parallel build
  BUILD_COMMAND
    ${CMAKE_COMMAND} --build <BINARY_DIR>
    --config Release
    --parallel

  INSTALL_COMMAND
    ${CMAKE_COMMAND} --install <BINARY_DIR> --config Release

  # Show cmake output so the user sees progress during R CMD INSTALL
  LOG_CONFIGURE FALSE
  LOG_BUILD     FALSE
  LOG_INSTALL   FALSE
)
