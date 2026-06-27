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

## Examples

``` r
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
result <- mesh_check(mesh)
```
