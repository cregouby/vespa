## R CMD check results

0 errors | 1 warnings | 1 note

### Warning about compiled code symbols

The warning about `_ZSt4cerr`, `_ZSt4cout`, `abort`, and `exit` symbols in the 
compiled DLL is a false positive. These symbols are referenced by the VTK and 
CGAL libraries (third-party dependencies), not by our package code. Our code 
uses `Rcpp::Rcerr`/`Rcpp::Rcout` for console output and `Rcpp::stop()` for 
error handling, as required by CRAN policies.

### Note 

* This is a new submission.
