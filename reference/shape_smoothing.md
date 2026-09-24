# Shape-preserving smoothing of a triangular surface mesh

Smooths a triangulated surface while preserving its overall shape, using
CGAL's mean curvature flow algorithm.

## Usage

``` r
shape_smoothing(
  mesh,
  n_iterations = 1L,
  time_step = 1e-04,
  update_attributes = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object.

- n_iterations:

  Number of smoothing iterations. Default `1L`.

- time_step:

  Controls the smoothing speed. Smaller values give a more conservative
  smoothing. Default `1e-4`.

- update_attributes:

  Whether to interpolate point and cell data from the input. Default
  `TRUE`.

## Value

A `mesh3d` object with the smoothed surface.

## See also

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
[`poisson_reconstruction()`](https://cregouby.github.io/vespa/reference/poisson_reconstruction.md),
[`region_fairing()`](https://cregouby.github.io/vespa/reference/region_fairing.md)

## Examples

``` r
# \donttest{
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
result <- shape_smoothing(mesh, n_iterations = 2L)
# }
```
