# Fair a region of a triangular surface mesh

Smooths a user-defined region of a triangulated surface mesh using
CGAL's region fairing algorithm. The region vertices are moved to
minimise a local fairing energy while the boundary of the region is kept
fixed.

## Usage

``` r
region_fairing(mesh, point_ids, update_attributes = TRUE)
```

## Arguments

- mesh:

  A `mesh3d` object.

- point_ids:

  Integer vector of 1-based vertex indices defining the region to be
  faired.

- update_attributes:

  Whether to interpolate point and cell data from the input. Default
  `TRUE`.

## Value

A `mesh3d` object with the selected region faired.

## Examples

``` r
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
result <- region_fairing(mesh, point_ids = 1:5)
```
