## R CMD check results

0 errors | 0 warnings | 0 notes

* This is a new submission.
* Checked with R 4.6.1 on Ubuntu 24.04 using `R CMD check --as-cran`.

## System requirements

This package links to the 'Vespa' C++ library, which in turn requires
CGAL (>= 5.5), Ceres Solver (>= 2.0) and VTK (>= 9.4). VTK is provided
through the 'rvtk' package, which uses a system installation when
available and otherwise supplies pre-built static libraries. The 'Vespa'
C++ library itself is not distributed on CRAN and is located by
`configure` through `VESPA_ROOT` or standard installation prefixes.

## Method references

There are no published references describing the methods in this
package. The package provides R bindings to existing CGAL/VTK mesh
processing algorithms.
