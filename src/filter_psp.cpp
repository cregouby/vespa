#include "converters.h"

#include <vtkCGALPCAEstimateNormals.h>
#include <vtkCGALXYZReader.h>
#include <vtkNew.h>
#include <vtkPolyData.h>

// [[Rcpp::export]]
Rcpp::List rcpp_pca_normals(
    Rcpp::List   mesh,
    unsigned int n_neighbors       = 18,
    bool         orient            = true,
    bool         delete_unoriented = true)
{
    auto input = vespa::mesh3d_to_vtk_with_normals(mesh);

    vtkNew<vtkCGALPCAEstimateNormals> filter;
    filter->SetInputData(input);
    filter->SetNumberOfNeighbors(n_neighbors);
    filter->SetOrientNormals(orient);
    filter->SetDeleteUnoriented(delete_unoriented);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "PCANormals");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("PCANormals produced empty output");

    return vespa::vtk_to_pointcloud(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_read_points(std::string filename)
{
    vtkNew<vtkCGALXYZReader> reader;
    reader->SetFileName(filename.c_str());

    vespa::VtkError err;
    vespa::install_error_observer(reader, err);
    reader->Update();
    vespa::check_vtk_error(err, "XYZReader");

    vtkPolyData* out = reader->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("XYZReader produced empty output");

    return vespa::vtk_to_pointcloud(out);
}
