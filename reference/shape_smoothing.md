# Shape-preserving smoothing of a triangular surface mesh

Smooths a triangulated surface while preserving its overall shape, using
CGAL's mean curvature flow algorithm.

## Usage

``` r
shape_smoothing(
  mesh,
  n_iterations = 1L,
  time_step = 1e-04,
  update_attributes = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object.

- n_iterations:

  Number of smoothing iterations. Default `1L`.

- time_step:

  Controls the smoothing speed. Smaller values give a more conservative
  smoothing. Default `1e-4`.

- update_attributes:

  Whether to interpolate point and cell data from the input. Default
  `TRUE`.

## Value

A `mesh3d` object with the smoothed surface.

## Examples

``` r
# \donttest{
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
result <- shape_smoothing(mesh, n_iterations = 2L)
# }
```
