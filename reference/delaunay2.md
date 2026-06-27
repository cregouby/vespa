# 2D Delaunay triangulation of a planar point set

Computes the Delaunay triangulation of a set of 2D points using CGAL's
constrained Delaunay algorithm. Optional edge constraints force specific
edges into the triangulation.

## Usage

``` r
delaunay2(points, constraints = NULL)
```

## Arguments

- points:

  A numeric matrix with 2 columns (x, y), one row per point.

- constraints:

  An optional integer matrix with 2 columns of 1-based vertex indices,
  one row per constraint edge. Default `NULL` (unconstrained Delaunay
  triangulation).

## Value

An `sfc_POLYGON` object (from the sf package) containing one triangle
polygon per Delaunay triangle.

## Examples

``` r
pts <- matrix(c(0,0, 1,0, 1,1, 0,1, 0.5,0.5), ncol = 2, byrow = TRUE)
triangles <- delaunay2(pts)
length(triangles)  # number of Delaunay triangles
#> [1] 4
```
