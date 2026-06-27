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

## Examples

``` r
# \donttest{
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
wrapped <- alpha_wrapping(mesh, alpha = 0.5, absolute_thresholds = TRUE)
# }
```
