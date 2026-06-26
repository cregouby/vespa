#include "converters.h"

#include <vtkCGALAdvancingFrontSurfaceReconstruction.h>
#include <vtkCGALPoissonSurfaceReconstructionDelaunay.h>
#include <vtkNew.h>
#include <vtkPolyData.h>

// [[Rcpp::export]]
Rcpp::List rcpp_poisson_recon(
    Rcpp::List mesh,
    double min_angle   = 20.0,
    double max_size    = 2.0,
    double distance    = 0.375,
    bool   gen_normals = true)
{
    auto input = vespa::mesh3d_to_vtk_with_normals(mesh);

    vtkNew<vtkCGALPoissonSurfaceReconstructionDelaunay> filter;
    filter->SetInputData(input);
    filter->SetMinTriangleAngle(min_angle);
    filter->SetMaxTriangleSize(max_size);
    filter->SetDistance(distance);
    filter->SetGenerateSurfaceNormals(gen_normals);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "PoissonRecon");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("PoissonRecon produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_advancing_front(
    Rcpp::List mesh,
    double per               = 0.0,
    double radius_ratio_bound = 5.0)
{
    auto input = vespa::mesh3d_to_vtk_with_normals(mesh);

    vtkNew<vtkCGALAdvancingFrontSurfaceReconstruction> filter;
    filter->SetInputData(input);
    filter->SetPer(per);
    filter->SetRadiusRatioBound(radius_ratio_bound);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "AdvancingFront");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("AdvancingFront produced empty output");

    return vespa::vtk_to_mesh3d(out);
}
