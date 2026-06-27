# Estimate point cloud normals via PCA

Estimates and orients surface normals for an unoriented point cloud
using CGAL's PCA-based normal estimation algorithm.

## Usage

``` r
pca_estimate_normals(
  mesh,
  n_neighbors = 18L,
  orient = TRUE,
  delete_unoriented = TRUE
)
```

## Arguments

- mesh:

  A `mesh3d` object representing a point cloud (may have an empty `$it`
  face matrix).

- n_neighbors:

  Number of nearest neighbours used for local PCA. Default `18L`.

- orient:

  Whether to orient the estimated normals consistently. Default `TRUE`.

- delete_unoriented:

  Whether to remove points whose normals could not be reliably oriented.
  Default `TRUE`.

## Value

A `mesh3d` object identical to the input but with an additional
`$normals` field (3×N numeric matrix, one column per point).

## See also

[`poisson_reconstruction()`](https://cregouby.github.io/vespa/reference/poisson_reconstruction.md)

## Examples

``` r
# \donttest{
f     <- system.file("extdata", "torus.stl", package = "vespa")
mesh  <- read_stl(f)
cloud <- mesh; cloud$it <- matrix(integer(0), 3L, 0L)
cloud_n <- pca_estimate_normals(cloud)
# }
```
