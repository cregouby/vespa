# Extract an isosurface from a 3-D scalar field

Generic function that extracts an isosurface from a volumetric scalar
field. Currently implemented for objects of class
[`sdf_volume`](https://cregouby.github.io/vespa/reference/sdf_volume.md)
(returned by
[`signed_distance_function()`](https://cregouby.github.io/vespa/reference/signed_distance_function.md)).

## Usage

``` r
extract_isosurface(x, isovalue = 0, ...)

# S3 method for class 'sdf_volume'
extract_isosurface(x, isovalue = 0, ...)

# Default S3 method
extract_isosurface(x, isovalue = 0, ...)
```

## Arguments

- x:

  An object from which to extract an isosurface.

- isovalue:

  Numeric. The iso-level at which to extract the surface.

- ...:

  Additional arguments passed to methods.

## Value

A `mesh3d` object (from package **rgl**).

## See also

[`signed_distance_function()`](https://cregouby.github.io/vespa/reference/signed_distance_function.md)
to build an `sdf_volume`.
