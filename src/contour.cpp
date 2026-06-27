#include <Rcpp.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkFlyingEdges3D.h> // or vtkMarchingCubes.h
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
  
  // Create and populate the scalar array
  vtkSmartPointer<vtkFloatArray> scalars = vtkSmartPointer<vtkFloatArray>::New();
  scalars->SetNumberOfTuples(array.size());
  for (int i = 0; i < array.size(); i++) {
    scalars->SetValue(i, static_cast<float>(array[i]));
  }
  
  // Attach scalars to the image data
  image->GetPointData()->SetScalars(scalars);
  
  // Extract isosurface using Flying Edges
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
  
  // Triangles: 3 rows (no marker needed for rgl::mesh3d(triangles=...))
  Rcpp::IntegerMatrix it(3, n_cells);
  for (int i = 0; i < n_cells; i++) {
    vtkCell* cell = polydata->GetCell(i);
    if (cell->GetNumberOfPoints() == 3) {
      it(0, i) = cell->GetPointId(0) + 1;  // 1-based indexing
      it(1, i) = cell->GetPointId(1) + 1;
      it(2, i) = cell->GetPointId(2) + 1;
    }
  }
  
  return Rcpp::List::create(
    Rcpp::Named("vb") = vb,
    Rcpp::Named("it") = it
  );
}
