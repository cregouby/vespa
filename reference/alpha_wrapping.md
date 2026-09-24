# Generate a watertight mesh via alpha wrapping

Constructs a 2-manifold, watertight surface mesh from a point cloud or
triangle soup using CGAL's alpha wrapping algorithm.

## Usage

``` r
alpha_wrapping(
  mesh,
  alpha = 5,
  offset = 3,
  absolute_thresholds = FALSE,
  update_attributes = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object (input point cloud or triangle soup).

- alpha:

  Maximum circumradius of output faces. Smaller values produce tighter
  wraps that capture finer concavities. Default `5` (interpreted as 5%
  of the bounding-box diagonal when `absolute_thresholds = FALSE`).

- offset:

  Mesh dilatation amount. Default `3`.

- absolute_thresholds:

  If `TRUE`, `alpha` and `offset` are in the same units as the mesh
  coordinates. If `FALSE` (default), they are percentages of the
  bounding-box diagonal.

- update_attributes:

  Whether to interpolate point and cell data from the input. Default
  `TRUE`.

## Value

A `mesh3d` object representing the wrapped surface.

## See also

Other mesh_transforms:
[`boolean_operation()`](https://cregouby.github.io/vespa/reference/boolean_operation.md),
[`isotropic_remeshing()`](https://cregouby.github.io/vespa/reference/isotropic_remeshing.md),
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
wrapped <- alpha_wrapping(mesh, alpha = 0.5, absolute_thresholds = TRUE)
# }
```
