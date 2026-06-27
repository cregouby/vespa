# SDF volume objects

An `sdf_volume` is a 3-D scalar field defined on a regular grid. It is
typically produced by
[`signed_distance_function()`](https://cregouby.github.io/vespa/reference/signed_distance_function.md),
where negative values lie inside the source mesh and positive values
outside.

## Components

- `dims`:

  Integer vector of length 3: grid dimensions `(nx, ny, nz)`.

- `spacing`:

  Numeric vector of length 3: voxel size along each axis.

- `origin`:

  Numeric vector of length 3: world-space origin of the grid.

- `array`:

  3-D numeric array of signed distances (x fastest).

## See also

[`signed_distance_function()`](https://cregouby.github.io/vespa/reference/signed_distance_function.md),
[`extract_isosurface()`](https://cregouby.github.io/vespa/reference/extract_isosurface.md)
