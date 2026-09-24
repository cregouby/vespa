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

Other mesh_transforms:
[`alpha_wrapping()`](https://cregouby.github.io/vespa/reference/alpha_wrapping.md),
[`boolean_operation()`](https://cregouby.github.io/vespa/reference/boolean_operation.md),
[`isotropic_remeshing()`](https://cregouby.github.io/vespa/reference/isotropic_remeshing.md),
[`mesh_check()`](https://cregouby.github.io/vespa/reference/mesh_check.md),
[`mesh_deformation()`](https://cregouby.github.io/vespa/reference/mesh_deformation.md),
[`mesh_smoothing()`](https://cregouby.github.io/vespa/reference/mesh_smoothing.md),
[`mesh_subdivision()`](https://cregouby.github.io/vespa/reference/mesh_subdivision.md),
[`patch_filling()`](https://cregouby.github.io/vespa/reference/patch_filling.md),
[`pca_estimate_normals()`](https://cregouby.github.io/vespa/reference/pca_estimate_normals.md),
[`region_fairing()`](https://cregouby.github.io/vespa/reference/region_fairing.md),
[`shape_smoothing()`](https://cregouby.github.io/vespa/reference/shape_smoothing.md)

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
