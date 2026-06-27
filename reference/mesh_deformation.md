# Deform a triangular surface mesh

Deforms a triangulated surface mesh by moving a set of control vertices
to specified target positions, propagating the deformation across the
surface using CGAL's as-rigid-as-possible (ARAP) or smoothed rotation
enhanced ARAP (SRE-ARAP) algorithms.

## Usage

``` r
mesh_deformation(
  mesh,
  control_ids,
  target_coords,
  roi_ids = integer(0),
  mode = c("smooth", "sre_arap"),
  sre_alpha = 0.02,
  n_iterations = 5L,
  tolerance = 1e-04
)
```

## Arguments

- mesh:

  A `mesh3d` object.

- control_ids:

  Integer vector of 1-based vertex indices that act as handles.

- target_coords:

  Numeric matrix with `length(control_ids)` rows and 3 columns (x, y,
  z). Target world-space positions for each handle.

- roi_ids:

  Integer vector of 1-based vertex indices forming the region of
  influence. Use `integer(0)` (the default) to let the filter determine
  the region automatically.

- mode:

  Deformation algorithm: `"smooth"` (ARAP, default) or `"sre_arap"`
  (Smoothed Rotation Enhanced ARAP).

- sre_alpha:

  Blending weight for SRE-ARAP (0 = pure ARAP, 1 = pure shape). Default
  `0.02`.

- n_iterations:

  Number of optimisation iterations. Default `5L`.

- tolerance:

  Convergence tolerance. Default `1e-4`.

## Value

A `mesh3d` object with the deformed surface.

## Examples

``` r
# \donttest{
f <- system.file("extdata", "torus.stl", package = "vespa")
mesh <- read_stl(f)
orig   <- mesh$vb[1:3, 1]
target <- matrix(orig + c(0.1, 0, 0), nrow = 1)
result <- mesh_deformation(mesh, control_ids = 1L, target_coords = target)
# }
```
