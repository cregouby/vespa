# Changelog

## vespa 0.6.0

- Reorganize documentation website.
- Add CI/CD workflows relying on exposed VTK 9.7 / 9.6 / 9.5 builds
  ([\#1](https://github.com/cregouby/vespa/issues/1))
- add 2 vignettes to website
- Fix Windows R CMD check “compiled code” warning: under header-only
  CGAL 6.x (rtools45/msys2), the CGAL assertion handler and CORE
  number-type headers emit `std::cerr`/`std::cout`/`abort`/`exit`
  references into the package’s own objects. `configure.win` now
  generates a console-output shim forwarding those messages to
  `REprintf()` (R API) and dropping the `abort()`/`exit()` branches so
  violations fall through to the existing `throw` path.

## vespa 0.5.0

- Add BUILD vespa lib capability.
- Bump vespa to development version.

## vespa 0.4.0

- Add
  [`extract_isosurface()`](https://cregouby.github.io/vespa/reference/extract_isosurface.md)
  for `sdf_volume` objects.
- [`signed_distance_function()`](https://cregouby.github.io/vespa/reference/signed_distance_function.md)
  now return a `sdf_volume` object.
- Add Signed Distance Function and Poisson Surface Reconstruction
  vignettes.

## vespa 0.3.0

- Renaming of all exported functions for better transparency.

## vespa 0.2.0

- Bump to vespa version 1.0.0.

## vespa 0.1.0

- Initial vespa wrapper.
