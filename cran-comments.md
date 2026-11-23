## R CMD check results

0 errors | 0 warnings | 0 notes

* This is a new submission.
* Checked with R 4.6.1 on Ubuntu 24.04 using `R CMD check --as-cran`.

## System requirements

This package requires CGAL (>= 5.5), Ceres Solver (>= 2.0), Eigen (>= 3.2),
Boost headers, and VTK (>= 9.4). VTK is provided through the 'rvtk' package,
which uses a system installation when available and otherwise supplies
pre-built static libraries. The 'Vespa' C++ sources are bundled in
`tools/vespa-src` (BSD-3-Clause, see `tools/vespa-src/License.txt` and
`inst/COPYRIGHTS`) and are compiled at install time. `configure` does not
access the network.

## Method references

There are no published references describing the methods in this
package. The package provides R bindings to existing CGAL/VTK mesh
processing algorithms.
