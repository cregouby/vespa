[![pipeline status](https://gitlab.kitware.com/vtk-cgal/vespa/badges/master/pipeline.svg)](https://gitlab.kitware.com/vtk-cgal/vespa/-/commits/master)

# VESPA - VTK Enhanced with Surface Processing Algorithms

VESPA is a ParaView plugin and VTK module that wraps the [CGAL library](https://www.cgal.org/). It enables the use of geometry and surface processing algorithms directly into your ParaView and VTK workflows.

<img src="Data/Resources/example0.png" width="460" />

*Highlighting the Isotropic Remesher filter*

<img src="Data/Resources/example1_1.png" width="230" />
<img src="Data/Resources/example1_2.png" width="230" />

*Surface Reconstruction from a point cloud*

<img src="Data/Resources/example2_1.png" width="230" />
<img src="Data/Resources/example2_2.png" width="230" />

*As-Rigid-As-Possible mesh deformation*

Go to the [download page](https://gitlab.kitware.com/vtk/meshing/vespa/-/releases) to try it out!

This project is distributed under a BSD-3 License, but as it is linked to CGAL, any binary generated with it retains the GPLv3 license, unless you get a commercial license from [GeometryFactory](https://geometryfactory.com/).

# Quickstart

To use VESPA as a ParaView plugin, download or compile it (see instructions below), then load it with `Tools` -> `Manage Plugins...` -> `Load New ...`

You now have access to the VESPA filters. You can see them all in `Filters` -> `VESPA`

# How to build

The VESPA project requires the following software:

* the [CMake](https://cmake.org/) build system,
* the [VTK library](https://vtk.org/) for the module or [ParaView software](https://www.paraview.org/) for the plugin,
* the [CGAL library](https://www.cgal.org/).

### VTK

VESPA needs VTK >= 9.0. It can be installed:
* using the package manager of your system (including brew on OSX, or vcpkg on Windows),
* manually using [CMake instructions](https://vtk.org/Wiki/VTK/Configure_and_Build).

### ParaView

VESPA needs ParaView >= 5.10.0. In order to build plugins, ParaView needs to
be compiled from sources manually using these
[CMake instructions](https://gitlab.kitware.com/paraview/paraview/-/blob/master/Documentation/dev/build.md).

### CGAL

VESPA needs CGAL >= 5.3. It can be installed:
* using the package manager of your system (including brew on OSX, or vcpkg on Windows),
* manually using [CMake instructions](https://doc.cgal.org/latest/Manual/installation.html#installation_configwithcmake).

If you want to get the [Alpha Wrapping](https://doc.cgal.org/latest/Alpha_wrap_3/index.html#Chapter_3D_Alpha_wrapping)
module, CGAL >= 5.5 is required.

### VESPA

VESPA can be installed using the standard CMake procedure:

1. Create a build folder
1. Launch CMake with this repository as source folder
1. Configure the project. It will need CGAL, as well as VTK or ParaView
depending on whether only the VTK module or the full ParaView plugin should
be built.

If you want to build the ParaView plugin, set the CMake variable `VESPA_BUILD_PV_PLUGIN` to `ON`.
   - If you have installed a library in a custom folder, you can find it in CMake
     by giving the folder: **install_dir**/lib/cmake/**project-version**. For example,
     for VTK: **install_dir**/lib/cmake/vtk-9.1.
   - You can provide a custom `CMAKE_INSTALL_PREFIX` if you do not want to install
     VESPA system wide.
   - Build and install VESPA (please refer to the
   [VTK tutorial](https://vtk.org/Wiki/VTK/Configure_and_Build#Build_VTK)
   if you do not know how to proceed).

# Usage notes

Except when stated otherwise, filters provided by VESPA require triangulated
surfaces (`vtkPolyData` meshes). These should be watertight and 2-manifold.

### VTK Module

With VTK, you may use the
[vtkTriangleFilter](https://vtk.org/doc/nightly/html/classvtkTriangleFilter.html)
and the
[vtkGeometryFilter](https://vtk.org/doc/nightly/html/classvtkGeometryFilter.html)
to get a valid triangulation. The `vtkCGALAlphaWrapping ` filter can be used
to ensure watertightness and 2-manifoldness.

#### Code examples

The testing of the project may be used to get simple examples on how to use each
provided filters. For instance,
for the [Isotropic remeshing](https://doc.cgal.org/latest/Polygon_mesh_processing/group__PMP__meshing__grp.html#gaa5cc92275df27f0baab2472ecbc4ea3f)
you get a [C++ example](./vespa/PolygonMeshProcessing/Testing/TestPMPIsotropicExecution.cxx)
and a [Python example](./vespa/PolygonMeshProcessing/Testing/execute_IsotropicRemesher.py).

Set the CMake variable `BUILD_TESTING` to `ON`, and pull the data files using `git lfs`.

### ParaView plugin

On ParaView, you may apply a `Tetrahedralize` and a `Extract Surface` filters
to get a valid triangulation. The `VESPA Alpha Wrapping` filter can be used
to ensure watertight, 2-manifold mesh then.

# How to contribute

If you want to contribute to this repository, simply open a Merge request
on the [gitlab instance](https://gitlab.kitware.com/vtk-cgal/vespa), we will
gladly review and help you merge your code.
