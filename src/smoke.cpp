#include <Rcpp.h>
#include <vtkObject.h>

// [[Rcpp::export]]
bool rcpp_vtk_available() {
  return true;
}
