# Reconstruct a surface from an oriented point cloud (Poisson)

Reconstructs a closed triangulated surface from an oriented point cloud
using CGAL's Poisson surface reconstruction algorithm. The input point
cloud must carry estimated normals (e.g., from
[`pca_estimate_normals()`](https://cregouby.github.io/vespa/reference/pca_estimate_normals.md)).

## Usage

``` r
poisson_reconstruction(
  mesh,
  min_angle = 20,
  max_size = 2,
  distance = 0.375,
  gen_normals = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object representing a point cloud. Must contain a
  `$normals` field (3×N matrix) produced by
  [`pca_estimate_normals()`](https://cregouby.github.io/vespa/reference/pca_estimate_normals.md).

- min_angle:

  Minimum triangle angle in the output mesh (degrees). Default `20`.

- max_size:

  Maximum triangle size relative to the average point spacing. Default
  `2`.

- distance:

  Maximum distance from the reconstructed surface to the input points,
  relative to average spacing. Default `0.375`.

- gen_normals:

  Whether to generate normals on the output surface. Default `TRUE`.

## Value

A `mesh3d` object with the reconstructed surface.

## See also

[`pca_estimate_normals()`](https://cregouby.github.io/vespa/reference/pca_estimate_normals.md),
[`advancing_front_reconstruction()`](https://cregouby.github.io/vespa/reference/advancing_front_reconstruction.md)

## Examples

``` r
# \donttest{
f     <- system.file("extdata", "torus.stl", package = "vespa")
mesh  <- read_stl(f)
cloud <- mesh; cloud$it <- matrix(integer(0), 3L, 0L)
cloud_n <- pca_estimate_normals(cloud)
result  <- poisson_reconstruction(cloud_n)
# }
```
