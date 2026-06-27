#include <Rcpp.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkFlyingEdges3D.h>  // ou vtkMarchingCubes.h
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkFloatArray.h>

// [[Rcpp::export]]
Rcpp::List extract_isosurface_cpp(Rcpp::NumericVector array,
                                  Rcpp::IntegerVector dims,
                                  Rcpp::NumericVector spacing,
                                  Rcpp::NumericVector origin,
                                  double isovalue) {
  
  // Create VTK ImageData from the array
  vtkSmartPointer<vtkImageData> image = vtkSmartPointer<vtkImageData>::New();
  image->SetDimensions(dims[0], dims[1], dims[2]);
  image->SetSpacing(spacing[0], spacing[1], spacing[2]);
  image->SetOrigin(origin[0], origin[1], origin[2]);
  
  // Copy array data (VTK expects Fortran order, which matches R)
  image->GetPointData()->SetScalars(
      vtkSmartPointer<vtkFloatArray>::New()
  );
  
  vtkFloatArray* scalars = vtkFloatArray::SafeDownCast(
    image->GetPointData()->GetScalars()
  );
  scalars->SetNumberOfTuples(array.size());
  for (int i = 0; i < array.size(); i++) {
    scalars->SetValue(i, array[i]);
  }
  
  // Extract isosurface using Flying Edges (faster than Marching Cubes)
  vtkSmartPointer<vtkFlyingEdges3D> contour = vtkSmartPointer<vtkFlyingEdges3D>::New();
  contour->SetInputData(image);
  contour->SetValue(0, isovalue);
  contour->Update();
  
  vtkPolyData* polydata = contour->GetOutput();
  
  // Extract vertices and triangles
  int n_points = polydata->GetNumberOfPoints();
  int n_cells = polydata->GetNumberOfCells();
  
  Rcpp::NumericMatrix vb(4, n_points);
  Rcpp::IntegerMatrix ib(4, n_cells);
  
  // Vertices (homogeneous coordinates)
  for (int i = 0; i < n_points; i++) {
    double p[3];
    polydata->GetPoint(i, p);
    vb(0, i) = p[0];
    vb(1, i) = p[1];
    vb(2, i) = p[2];
    vb(3, i) = 1.0;
  }
  
  // Triangles (indices start at 0 for R)
  for (int i = 0; i < n_cells; i++) {
    vtkCell* cell = polydata->GetCell(i);
    if (cell->GetNumberOfPoints() == 3) {
      ib(0, i) = cell->GetPointId(0) + 1;  // R uses 1-based indexing
      ib(1, i) = cell->GetPointId(1) + 1;
      ib(2, i) = cell->GetPointId(2) + 1;
      ib(3, i) = 3;  // Triangle marker
    }
  }
  
  return Rcpp::List::create(
    Rcpp::Named("vb") = vb,
    Rcpp::Named("ib") = ib
  );
}