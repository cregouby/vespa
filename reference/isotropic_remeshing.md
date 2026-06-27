# Isotropic remeshing of a triangular surface mesh

Remeshes a triangulated surface so that all edges approach a uniform
target length, using CGAL's isotropic remeshing algorithm.

## Usage

``` r
isotropic_remeshing(
  mesh,
  target_length = -1,
  protect_angle = 45,
  n_iterations = 1L,
  update_attributes = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object (from the rgl package).

- target_length:

  Target edge length for the result. A value of `-1` (default) lets
  Vespa choose automatically (1% of the bounding box diagonal).

- protect_angle:

  Feature edges whose dihedral angle exceeds this threshold (degrees)
  are preserved. Default `45`.

- n_iterations:

  Number of remeshing iterations. Default `1`.

- update_attributes:

  Whether to interpolate point and cell data arrays from the input mesh.
  Default `TRUE`.

## Value

A `mesh3d` object with the remeshed surface.

## Examples

``` r
# \donttest{
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
result <- isotropic_remeshing(mesh, n_iterations = 1L)
# }
```
