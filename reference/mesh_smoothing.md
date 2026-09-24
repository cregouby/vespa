# Smooth a triangular surface mesh

Smooths a triangulated surface using CGAL's polygon mesh processing
smoothing algorithms.

## Usage

``` r
mesh_smoothing(
  mesh,
  method = c("tangential", "angle_area"),
  n_iterations = 10L,
  safety_constraints = FALSE,
  update_attributes = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object.

- method:

  Smoothing method: `"tangential"` (default) for tangential relaxation,
  or `"angle_area"` for angle and area regularisation.

- n_iterations:

  Number of smoothing iterations. Default `10L`.

- safety_constraints:

  If `TRUE`, applies safety constraints to limit vertex displacement.
  Default `FALSE`.

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
[`mesh_subdivision()`](https://cregouby.github.io/vespa/reference/mesh_subdivision.md),
[`patch_filling()`](https://cregouby.github.io/vespa/reference/patch_filling.md),
[`pca_estimate_normals()`](https://cregouby.github.io/vespa/reference/pca_estimate_normals.md),
[`poisson_reconstruction()`](https://cregouby.github.io/vespa/reference/poisson_reconstruction.md),
[`region_fairing()`](https://cregouby.github.io/vespa/reference/region_fairing.md),
[`shape_smoothing()`](https://cregouby.github.io/vespa/reference/shape_smoothing.md)

## Examples

``` r
# \donttest{
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
smooth <- mesh_smoothing(mesh, method = "tangential", n_iterations = 3L)
# }
```
