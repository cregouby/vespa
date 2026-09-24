# Estimate point cloud normals via PCA

Estimates and orients surface normals for an unoriented point cloud
using CGAL's PCA-based normal estimation algorithm.

## Usage

``` r
pca_estimate_normals(
  mesh,
  n_neighbors = 18L,
  orient = TRUE,
  delete_unoriented = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object representing a point cloud (may have an empty `$it`
  face matrix).

- n_neighbors:

  Number of nearest neighbours used for local PCA. Default `18L`.

- orient:

  Whether to orient the estimated normals consistently. Default `TRUE`.

- delete_unoriented:

  Whether to remove points whose normals could not be reliably oriented.
  Default `TRUE`.

## Value

A `mesh3d` object identical to the input but with an additional
`$normals` field (3×N numeric matrix, one column per point).

## See also

[`poisson_reconstruction()`](https://cregouby.github.io/vespa/reference/poisson_reconstruction.md)

Other mesh_transforms:
[`alpha_wrapping()`](https://cregouby.github.io/vespa/reference/alpha_wrapping.md),
[`boolean_operation()`](https://cregouby.github.io/vespa/reference/boolean_operation.md),
[`isotropic_remeshing()`](https://cregouby.github.io/vespa/reference/isotropic_remeshing.md),
[`mesh_check()`](https://cregouby.github.io/vespa/reference/mesh_check.md),
[`mesh_deformation()`](https://cregouby.github.io/vespa/reference/mesh_deformation.md),
[`mesh_smoothing()`](https://cregouby.github.io/vespa/reference/mesh_smoothing.md),
[`mesh_subdivision()`](https://cregouby.github.io/vespa/reference/mesh_subdivision.md),
[`patch_filling()`](https://cregouby.github.io/vespa/reference/patch_filling.md),
[`poisson_reconstruction()`](https://cregouby.github.io/vespa/reference/poisson_reconstruction.md),
[`region_fairing()`](https://cregouby.github.io/vespa/reference/region_fairing.md),
[`shape_smoothing()`](https://cregouby.github.io/vespa/reference/shape_smoothing.md)

## Examples

``` r
# \donttest{
f     <- system.file("extdata", "torus.stl", package = "vespa")
mesh  <- read_stl(f)
cloud <- mesh; cloud$it <- matrix(integer(0), 3L, 0L)
cloud_n <- pca_estimate_normals(cloud)
# }
```
