# Fill holes in a triangular surface mesh

Detects and fills holes in a triangulated surface mesh using CGAL's hole
filling algorithm. When `point_ids` is supplied, only the hole whose
boundary passes through those points is filled; otherwise all detected
holes are filled.

## Usage

``` r
patch_filling(
  mesh,
  point_ids = integer(0),
  fairing_continuity = 1L,
  update_attributes = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object.

- point_ids:

  Integer vector of 1-based vertex indices on the hole boundary to fill.
  Use `integer(0)` (the default) to fill all holes.

- fairing_continuity:

  Integer controlling the continuity order of the filled patch: `0`
  (C0), `1` (C1, default), or `2` (C2).

- update_attributes:

  Whether to interpolate point and cell data from the input. Default
  `TRUE`.

## Value

A `mesh3d` object with the hole(s) filled.

## See also

Other mesh_transforms:
[`alpha_wrapping()`](https://cregouby.github.io/vespa/reference/alpha_wrapping.md),
[`boolean_operation()`](https://cregouby.github.io/vespa/reference/boolean_operation.md),
[`isotropic_remeshing()`](https://cregouby.github.io/vespa/reference/isotropic_remeshing.md),
[`mesh_check()`](https://cregouby.github.io/vespa/reference/mesh_check.md),
[`mesh_deformation()`](https://cregouby.github.io/vespa/reference/mesh_deformation.md),
[`mesh_smoothing()`](https://cregouby.github.io/vespa/reference/mesh_smoothing.md),
[`mesh_subdivision()`](https://cregouby.github.io/vespa/reference/mesh_subdivision.md),
[`pca_estimate_normals()`](https://cregouby.github.io/vespa/reference/pca_estimate_normals.md),
[`poisson_reconstruction()`](https://cregouby.github.io/vespa/reference/poisson_reconstruction.md),
[`region_fairing()`](https://cregouby.github.io/vespa/reference/region_fairing.md),
[`shape_smoothing()`](https://cregouby.github.io/vespa/reference/shape_smoothing.md)

## Examples

``` r
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
result <- patch_filling(mesh)  # no-op: torus has no holes
```
