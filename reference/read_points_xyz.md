# Read a point cloud file

Reads a point cloud from disk using CGAL's point set reader. Supported
formats: `.las`, `.off`, `.ply`, `.xyz`.

## Usage

``` r
read_points_xyz(filename)
```

## Arguments

- filename:

  Path to the point cloud file. The file must exist and be readable.

## Value

A `mesh3d` object representing the point cloud. `$vb` is a 4×N matrix of
homogeneous point coordinates; `$it` is an empty 3×0 matrix. If the file
contains normal vectors, they are returned in `$normals` (3×N matrix).

## Examples

``` r
f <- system.file("testdata", "test_points.xyz", package = "vespa")
cloud <- read_points_xyz(f)
print(cloud)
#> <mesh3d> 50 vertices 0 faces
```
