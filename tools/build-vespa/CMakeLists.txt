cmake_minimum_required(VERSION 3.18)
project(VespaBuild NONE)

include(ExternalProject)

# Installation prefix for Vespa build artifacts.
# When invoked from the configure script, this is set to <pkg_source>/inst/vespa.
set(INSTALL_PREFIX "${CMAKE_BINARY_DIR}/vespa-install"
    CACHE PATH "Destination directory for Vespa headers and libraries")

# Git tag or commit read from .github/vespa-version.txt and forwarded by configure.
# Default is the official v1.0 release tag; override with -DVESPA_GIT_TAG=<hash>.
set(VESPA_GIT_TAG "v1.0"
    CACHE STRING "Vespa git tag or commit hash to build")

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
  # Tags support shallow clone; raw hashes do not
  GIT_SHALLOW    TRUE
  GIT_PROGRESS   TRUE

  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
    ${_CC_ARG}
    ${_CXX_ARG}

  # Parallel build — honour MAKEFLAGS / NPROC if set, else auto-detect
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

# Convenience target name matching what configure invokes
add_custom_target(vespa-install DEPENDS vespa)
