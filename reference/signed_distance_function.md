# Compute a signed distance function (SDF) volume from a surface mesh

Generates a 3-D signed distance field from a triangulated surface mesh
using CGAL's signed distance function. Negative values are inside the
mesh, positive values are outside.

## Usage

``` r
signed_distance_function(mesh, base_resolution = 64L, padding = 0L)
```

## Arguments

- mesh:

  A `mesh3d` object. The mesh should be closed and non-self-intersecting
  for physically meaningful signed distances.

- base_resolution:

  Number of grid nodes along the shortest bounding-box axis. Default
  `64L`.

- padding:

  Number of voxels of padding added on each side of each axis around the
  mesh bounding box. Default `0L`.

## Value

An object of class `sdf_volume` (a list subclass) with elements:

- `dims`:

  Integer vector of length 3: grid dimensions (nx, ny, nz).

- `spacing`:

  Numeric vector of length 3: voxel size in each direction.

- `origin`:

  Numeric vector of length 3: world-space origin of the grid.

- `array`:

  3-D numeric array of shape `dims` (x fastest, VTK/Fortran order)
  containing the signed distances. Use `as.vector(sdf$array)` to recover
  a flat vector.

This object is accepted by
[`extract_isosurface()`](https://cregouby.github.io/vespa/reference/extract_isosurface.md)
and has dedicated [`print()`](https://rdrr.io/r/base/print.html) and
[`plot()`](https://rdrr.io/r/graphics/plot.default.html) methods.

## Note

**Grid resolution and cubic voxels.** Let \\L\_{\min} = \min(L_x, L_y,
L_z)\\ be the shortest bounding-box extent. The voxel size is \$\$h =
\frac{L\_{\min}}{b - 1},\$\$ where \\b\\ is `base_resolution`. The
number of grid nodes along each axis is then \$\$N\_{\alpha} =
\Bigl\lfloor \frac{L\_{\alpha}}{h} \Bigr\rceil + 1 + 2\\p,\$\$ where
\\\lfloor\cdot\rceil\\ denotes rounding to the nearest integer and \\p\\
is `padding`. This guarantees that voxels are cubic regardless of the
mesh aspect ratio.

## See also

[`extract_isosurface()`](https://cregouby.github.io/vespa/reference/extract_isosurface.md)
to extract a `mesh3d` from an `sdf_volume`.

## Examples

``` r
# \donttest{
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
sdf  <- signed_distance_function(mesh, base_resolution = 32L)
#> SDF grid: 135 × 135 × 32 — 583,200 voxels, ~4.4 Mb
sdf                              # uses print.sdf_volume
#> <sdf_volume>
#>   dims:    135 x 135 x 32  (583,200 voxels)
#>   spacing: 0.0194 x 0.0193 x 0.01935
#>   origin:  (-1.3, -1.293, -0.3)
#>   values:  [-0.2899, 0.7404]
range(sdf$array)                 # negative inside, positive outside
#> [1] -0.2899068  0.7403798
# }
```
