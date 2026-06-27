# Boolean operation on two triangular surface meshes

Computes the union, intersection, or difference of two closed
triangulated surface meshes using CGAL's polygon mesh processing boolean
operations. Both meshes must be closed (watertight) and free of
self-intersections.

## Usage

``` r
boolean_operation(
  mesh_a,
  mesh_b,
  operation = c("difference", "intersection", "union"),
  update_attributes = TRUE
)
```

## Arguments

- mesh_a:

  A `mesh3d` object (first operand).

- mesh_b:

  A `mesh3d` object (second operand).

- operation:

  One of `"difference"` (default), `"intersection"`, or `"union"`. The
  difference is `mesh_a` minus `mesh_b`.

- update_attributes:

  Whether to interpolate point and cell data from the inputs. Default
  `TRUE`.

## Value

A `mesh3d` object representing the result of the boolean operation.

## Examples

``` r
# \donttest{
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh  <- read_stl(f)
mesh2 <- mesh
mesh2$vb[1, ] <- mesh2$vb[1, ] + 0.5  # shift copy along X
result <- boolean_operation(mesh, mesh2, operation = "union")
# }
```
