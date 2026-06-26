#pragma once

#include <Rcpp.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkSelection.h>
#include <vtkSmartPointer.h>

namespace vespa {

// mesh3d ($vb 4×N double, $it 3×M int, 1-based) → vtkPolyData (with cells)
vtkSmartPointer<vtkPolyData> mesh3d_to_vtk(const Rcpp::List& mesh);

// mesh3d or point cloud → vtkPolyData (points only + optional $normals 3×N)
vtkSmartPointer<vtkPolyData> mesh3d_to_vtk_with_normals(const Rcpp::List& mesh);

// vtkPolyData → mesh3d list with class c("mesh3d","shape3d")
Rcpp::List vtk_to_mesh3d(vtkPolyData* poly);

// vtkPolyData (point cloud) → mesh3d list with empty $it and optional $normals
Rcpp::List vtk_to_pointcloud(vtkPolyData* poly);

// vtkImageData → list(dims, spacing, origin, values)
Rcpp::List vtk_imagedata_to_r(vtkImageData* img);

// vtkPolyData (triangles) → list of 4×2 closed XY ring matrices (for sf)
Rcpp::List vtk_to_triangle_rings(vtkPolyData* poly);

// 1-based integer vector of point IDs → vtkSelection (0-based internally)
vtkSmartPointer<vtkSelection> ids_to_vtk_selection(
    const Rcpp::IntegerVector& ids, bool field_is_point = true);

// VTK error context — fill via install_error_observer, check via check_vtk_error
struct VtkError {
    bool        occurred = false;
    std::string message;
};

unsigned long install_error_observer(vtkObject* obj, VtkError& ctx);
void check_vtk_error(const VtkError& err, const char* filter_name);

// VTK warning context — collect via install_warning_observer, emit via emit_vtk_warnings
struct VtkWarnings {
    std::vector<std::string> messages;
};

unsigned long install_warning_observer(vtkObject* obj, VtkWarnings& ctx);
void emit_vtk_warnings(const VtkWarnings& w);

} // namespace vespa
