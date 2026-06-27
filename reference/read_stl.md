# Read a surface mesh from an STL file

Reads a triangulated surface mesh from an ASCII or binary STL file using
VTK's STL reader.

## Usage

``` r
read_stl(filename)
```

## Arguments

- filename:

  Path to the STL file. The file must exist and be readable.

## Value

A `mesh3d` object. `$vb` is a 4×N matrix of homogeneous vertex
coordinates; `$it` is a 3×M matrix of 1-based triangle indices.

## Examples

``` r
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
print(mesh)
#> <mesh3d> 1800 vertices 3600 faces
```
