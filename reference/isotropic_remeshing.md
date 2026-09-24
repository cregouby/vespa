# Isotropic remeshing of a triangular surface mesh

Remeshes a triangulated surface so that all edges approach a uniform
target length, using CGAL's isotropic remeshing algorithm.

## Usage

``` r
isotropic_remeshing(
  mesh,
  target_length = -1,
  protect_angle = 45,
  n_iterations = 1L,
  update_attributes = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object (from the rgl package).

- target_length:

  Target edge length for the result. A value of `-1` (default) lets
  Vespa choose automatically (1% of the bounding box diagonal).

- protect_angle:

  Feature edges whose dihedral angle exceeds this threshold (degrees)
  are preserved. Default `45`.

- n_iterations:

  Number of remeshing iterations. Default `1`.

- update_attributes:

  Whether to interpolate point and cell data arrays from the input mesh.
  Default `TRUE`.

## Value

A `mesh3d` object with the remeshed surface.

## See also

Other mesh_transforms:
[`alpha_wrapping()`](https://cregouby.github.io/vespa/reference/alpha_wrapping.md),
[`boolean_operation()`](https://cregouby.github.io/vespa/reference/boolean_operation.md),
[`mesh_check()`](https://cregouby.github.io/vespa/reference/mesh_check.md),
[`mesh_deformation()`](https://cregouby.github.io/vespa/reference/mesh_deformation.md),
[`mesh_smoothing()`](https://cregouby.github.io/vespa/reference/mesh_smoothing.md),
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
result <- isotropic_remeshing(mesh, n_iterations = 1L)
# }
```
