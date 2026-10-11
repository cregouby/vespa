cmake_minimum_required(VERSION 3.19)
project(VespaBuild NONE)

include(ExternalProject)

# ---------------------------------------------------------------------------
# Options avec défauts sûrs : statique + PIC, pour que tout soit plié dans
# vespa.so (une seule copie des objets VTK dans le processus R).
# ---------------------------------------------------------------------------
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type propagated to Vespa")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Vespa as shared libraries")
set(CMAKE_POSITION_INDEPENDENT_CODE ON
    CACHE BOOL "PIC (required: static libs linked into vespa.so)")

set(INSTALL_PREFIX "${CMAKE_BINARY_DIR}/vespa-install"
    CACHE PATH "Destination directory for Vespa headers and libraries")
set(VESPA_GIT_TAG "v1.0" CACHE STRING "Vespa git tag or commit hash to build")

# ---------------------------------------------------------------------------
# Résolution de VTK_ROOT : variable cache d'abord, sinon environnement (CI).
# ---------------------------------------------------------------------------
set(VTK_ROOT "" CACHE PATH "Prebuilt VTK install prefix")
if(NOT VTK_ROOT AND DEFINED ENV{VTK_ROOT})
  set(VTK_ROOT "$ENV{VTK_ROOT}")
endif()

message(STATUS "Superbuild VTK_ROOT    : '${VTK_ROOT}'")
message(STATUS "Superbuild INSTALL     : '${INSTALL_PREFIX}'")
message(STATUS "Superbuild SHARED / PIC: ${BUILD_SHARED_LIBS} / ${CMAKE_POSITION_INDEPENDENT_CODE}")

# ---------------------------------------------------------------------------
# CMAKE_PREFIX_PATH du sous-projet : VTK_ROOT d'abord, puis chemins système.
# ---------------------------------------------------------------------------
set(_prefixes "${CMAKE_PREFIX_PATH}")
if(VTK_ROOT)
  list(APPEND _prefixes "${VTK_ROOT}")
endif()
if(UNIX AND NOT APPLE)
  list(APPEND _prefixes "/usr" "/usr/local")
elseif(APPLE)
  list(APPEND _prefixes "/opt/homebrew" "/usr/local")
endif()
if(WIN32)
  list(APPEND _prefixes "C:/rtools45/ucrt64")
endif()
if(_prefixes)
  list(REMOVE_DUPLICATES _prefixes)
endif()
message(STATUS "Superbuild sub-prefixes: '${_prefixes}'")

# Échapper les ';' : sinon la liste éclate en plusieurs items de
# CMAKE_CACHE_ARGS et seul le premier chemin serait transmis.
string(REPLACE ";" "\\;" _prefixes_escaped "${_prefixes}")

# ---------------------------------------------------------------------------
# VTK_DIR explicite (ceinture + bretelles) + échec précoce lisible si le
# tarball n'est pas un arbre `cmake --install` complet.
# ---------------------------------------------------------------------------
set(_vtk_cache_args "")
if(VTK_ROOT)
  file(GLOB _vtk_cmake_dirs "${VTK_ROOT}/lib/cmake/vtk-*")
  if(_vtk_cmake_dirs)
    list(SORT _vtk_cmake_dirs)
    list(GET _vtk_cmake_dirs 0 _vtk_dir)
    message(STATUS "Superbuild vtk-config  : '${_vtk_dir}'")
    list(APPEND _vtk_cache_args "-DVTK_DIR:PATH=${_vtk_dir}")
  else()
    message(FATAL_ERROR
      "VTK_ROOT='${VTK_ROOT}' est défini mais aucun lib/cmake/vtk-*/vtk-config.cmake "
      "n'y a été trouvé. Le tarball doit être un arbre `cmake --install` complet "
      "(include/ + lib/*.a + lib/cmake/vtk-X.Y/).")
  endif()
endif()

# ---------------------------------------------------------------------------
# Propagation des compilateurs de la toolchain R (premier token uniquement).
# ---------------------------------------------------------------------------
set(_compiler_args "")
if(DEFINED ENV{CC})
  string(REGEX REPLACE "^[ ]*([^ ]+).*" "\\1" _cc "$ENV{CC}")
  list(APPEND _compiler_args "-DCMAKE_C_COMPILER=${_cc}")
endif()
if(DEFINED ENV{CXX})
  string(REGEX REPLACE "^[ ]*([^ ]+).*" "\\1" _cxx "$ENV{CXX}")
  list(APPEND _compiler_args "-DCMAKE_CXX_COMPILER=${_cxx}")
endif()

# Clone shallow uniquement pour les refs nommées, pas les hashes bruts.
string(LENGTH "${VESPA_GIT_TAG}" _tag_len)
if(_tag_len GREATER 15 AND "${VESPA_GIT_TAG}" MATCHES "^[0-9a-f]+$")
  set(_git_shallow FALSE)
else()
  set(_git_shallow TRUE)
endif()

ExternalProject_Add(vespa
  GIT_REPOSITORY "https://gitlab.kitware.com/vtk/meshing/vespa.git"
  GIT_TAG        "${VESPA_GIT_TAG}"
  GIT_SHALLOW    ${_git_shallow}
  GIT_PROGRESS   TRUE

  PATCH_COMMAND
    sed -i.bak
      -e "s|find_package(Eigen3 3\\.2\\.0 REQUIRED|find_package(Eigen3 REQUIRED|g"
      -e "s|find_package(CGAL 5\\.3\\.0 REQUIRED|find_package(CGAL REQUIRED|g"
      -e "s|option(BUILD_PYTHON_WRAPPERS \"Build python wrapping\" ON)|option(BUILD_PYTHON_WRAPPERS \"Build python wrapping\" OFF)|"
      <SOURCE_DIR>/CMakeLists.txt &&
    ${CMAKE_COMMAND} -E rm -f <SOURCE_DIR>/CMakeLists.txt.bak

  # Pas de -GNinja ici : le générateur est hérité du superbuild
  # (le doublon `-GNinja -GNinja` des logs venait de la redondance).
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
    -DBUILD_PYTHON_WRAPPERS=OFF
    -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
    -DCMAKE_POSITION_INDEPENDENT_CODE=${CMAKE_POSITION_INDEPENDENT_CODE}
    ${_compiler_args}

  CMAKE_CACHE_ARGS
    -DCMAKE_PREFIX_PATH:PATH=${_prefixes_escaped}
    ${_vtk_cache_args}

  BUILD_COMMAND
    ${CMAKE_COMMAND} --build <BINARY_DIR> --config Release --parallel
  INSTALL_COMMAND
    ${CMAKE_COMMAND} --install <BINARY_DIR> --config Release

  LOG_CONFIGURE FALSE
  LOG_BUILD     FALSE
  LOG_INSTALL   FALSE
)