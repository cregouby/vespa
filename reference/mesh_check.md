# Validate and optionally repair a triangular surface mesh

Checks a mesh for watertightness and self-intersections using CGAL's
polygon mesh processing algorithms. Issues are reported as R warnings.
When `attempt_repair = TRUE`, Vespa attempts to fix detected problems
and returns the repaired mesh.

## Usage

``` r
mesh_check(
  mesh,
  check_watertight = TRUE,
  check_intersect = TRUE,
  attempt_repair = FALSE
)
```

## Arguments

- mesh:

  A `mesh3d` object.

- check_watertight:

  Whether to check if the mesh is closed and bounds a volume. Default
  `TRUE`.

- check_intersect:

  Whether to check for self-intersections. Default `TRUE`.

- attempt_repair:

  If `TRUE`, Vespa attempts to fix detected issues (fill holes,
  re-orient faces, remove self-intersections). Default `FALSE`.

## Value

A `mesh3d` object — identical to the input unless
`attempt_repair = TRUE` and repairs were applied. R warnings are emitted
for each issue detected.

## See also

Other mesh_transforms:
[`alpha_wrapping()`](https://cregouby.github.io/vespa/reference/alpha_wrapping.md),
[`boolean_operation()`](https://cregouby.github.io/vespa/reference/boolean_operation.md),
[`isotropic_remeshing()`](https://cregouby.github.io/vespa/reference/isotropic_remeshing.md),
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
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
result <- mesh_check(mesh)
```
