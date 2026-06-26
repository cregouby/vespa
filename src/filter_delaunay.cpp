#include "converters.h"

#include <vtkCGALDelaunay2.h>
#include <vtkCellArray.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>

// [[Rcpp::export]]
Rcpp::List rcpp_delaunay2(
    Rcpp::NumericMatrix points,
    Rcpp::Nullable<Rcpp::IntegerMatrix> constraint_edges = R_NilValue)
{
    const int nv = points.nrow();

    vtkNew<vtkPoints> pts;
    pts->SetNumberOfPoints(nv);
    for (int i = 0; i < nv; ++i)
        pts->SetPoint(i, points(i, 0), points(i, 1), 0.0);

    vtkNew<vtkPolyData> input;
    input->SetPoints(pts);

    if (constraint_edges.isNotNull()) {
        Rcpp::IntegerMatrix edges(constraint_edges);
        vtkNew<vtkCellArray> lines;
        for (int j = 0; j < edges.nrow(); ++j) {
            vtkIdType ids[2] = {
                static_cast<vtkIdType>(edges(j, 0) - 1),
                static_cast<vtkIdType>(edges(j, 1) - 1)
            };
            lines->InsertNextCell(2, ids);
        }
        input->SetLines(lines);
    }

    vtkNew<vtkCGALDelaunay2> filter;
    filter->SetInputData(input);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "Delaunay2");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfCells() == 0)
        Rcpp::stop("Delaunay2 produced empty output");

    return vespa::vtk_to_triangle_rings(out);
}
