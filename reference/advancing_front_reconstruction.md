# Reconstruct a surface from a point cloud (Advancing Front)

Reconstructs a triangulated surface from an unoriented point cloud using
CGAL's advancing front surface reconstruction algorithm. Unlike Poisson
reconstruction, no normals are required.

## Usage

``` r
advancing_front_reconstruction(mesh, per = 0, radius_ratio_bound = 5)
```

## Arguments

- mesh:

  A `mesh3d` object representing a point cloud (may have an empty `$it`
  face matrix).

- per:

  Priority function parameter controlling the trade-off between
  advancing front size and point proximity. Default `0` (automatic).

- radius_ratio_bound:

  Maximum radius ratio of generated triangles (quality bound). Default
  `5`.

## Value

A `mesh3d` object with the reconstructed surface.

## See also

[`poisson_reconstruction()`](https://cregouby.github.io/vespa/reference/poisson_reconstruction.md)

## Examples

``` r
# \donttest{
f     <- system.file("extdata", "torus.stl", package = "vespa")
mesh  <- read_stl(f)
cloud <- mesh; cloud$it <- matrix(integer(0), 3L, 0L)
result <- advancing_front_reconstruction(cloud)
# }
```
