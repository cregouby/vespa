# Subdivide a triangular surface mesh

Applies a subdivision scheme to a triangulated surface mesh using CGAL's
polygon mesh processing subdivision algorithms.

## Usage

``` r
mesh_subdivision(
  mesh,
  type = c("sqrt3", "loop", "catmull_clark", "doo_sabin"),
  n_iterations = 1L,
  update_attributes = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object.

- type:

  Subdivision scheme: `"sqrt3"` (default), `"loop"`, `"catmull_clark"`,
  or `"doo_sabin"`.

- n_iterations:

  Number of subdivision iterations. Default `1L`.

- update_attributes:

  Whether to interpolate point and cell data from the input. Default
  `TRUE`.

## Value

A `mesh3d` object with the subdivided surface.

## See also

Other mesh_transforms:
[`alpha_wrapping()`](https://cregouby.github.io/vespa/reference/alpha_wrapping.md),
[`boolean_operation()`](https://cregouby.github.io/vespa/reference/boolean_operation.md),
[`isotropic_remeshing()`](https://cregouby.github.io/vespa/reference/isotropic_remeshing.md),
[`mesh_check()`](https://cregouby.github.io/vespa/reference/mesh_check.md),
[`mesh_deformation()`](https://cregouby.github.io/vespa/reference/mesh_deformation.md),
[`mesh_smoothing()`](https://cregouby.github.io/vespa/reference/mesh_smoothing.md),
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
sub <- mesh_subdivision(mesh, type = "loop", n_iterations = 1L)
print(sub)
#> <mesh3d> 7200 vertices 14400 faces
# }
```
