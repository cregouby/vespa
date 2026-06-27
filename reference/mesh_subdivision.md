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
