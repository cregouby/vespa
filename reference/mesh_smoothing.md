# Smooth a triangular surface mesh

Smooths a triangulated surface using CGAL's polygon mesh processing
smoothing algorithms.

## Usage

``` r
mesh_smoothing(
  mesh,
  method = c("tangential", "angle_area"),
  n_iterations = 10L,
  safety_constraints = FALSE,
  update_attributes = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object.

- method:

  Smoothing method: `"tangential"` (default) for tangential relaxation,
  or `"angle_area"` for angle and area regularisation.

- n_iterations:

  Number of smoothing iterations. Default `10L`.

- safety_constraints:

  If `TRUE`, applies safety constraints to limit vertex displacement.
  Default `FALSE`.

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
smooth <- mesh_smoothing(mesh, method = "tangential", n_iterations = 3L)
# }
```
