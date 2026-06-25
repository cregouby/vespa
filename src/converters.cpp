#include "converters.h"

#include <vtkCallbackCommand.h>
#include <vtkCellArray.h>
#include <vtkCommand.h>
#include <vtkDataArray.h>
#include <vtkIdList.h>
#include <vtkIdTypeArray.h>
#include <vtkInformation.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkSelectionNode.h>

#include <vtkFloatArray.h>

#include <string>

// mesh3d → vtkPolyData
vtkSmartPointer<vtkPolyData> rvespa::mesh3d_to_vtk(const Rcpp::List& mesh) {
    Rcpp::NumericMatrix vb = mesh["vb"]; // 4 × N  (x, y, z, w per column)
    Rcpp::IntegerMatrix it = mesh["it"]; // 3 × M  (1-based vertex indices)

    const int nv = vb.ncol();
    const int nf = it.ncol();

    vtkNew<vtkPoints> pts;
    pts->SetNumberOfPoints(nv);
    for (int i = 0; i < nv; ++i)
        pts->SetPoint(i, vb(0, i), vb(1, i), vb(2, i));

    vtkNew<vtkCellArray> cells;
    cells->AllocateEstimate(nf, 3);
    for (int j = 0; j < nf; ++j) {
        vtkIdType ids[3] = {
            static_cast<vtkIdType>(it(0, j) - 1),
            static_cast<vtkIdType>(it(1, j) - 1),
            static_cast<vtkIdType>(it(2, j) - 1)
        };
        cells->InsertNextCell(3, ids);
    }

    auto poly = vtkSmartPointer<vtkPolyData>::New();
    poly->SetPoints(pts);
    poly->SetPolys(cells);
    return poly;
}

// mesh3d or point cloud → vtkPolyData with points + optional normals from $normals (3×N)
vtkSmartPointer<vtkPolyData> rvespa::mesh3d_to_vtk_with_normals(const Rcpp::List& mesh) {
    Rcpp::NumericMatrix vb = mesh["vb"];
    const int nv = vb.ncol();

    vtkNew<vtkPoints> pts;
    pts->SetNumberOfPoints(nv);
    for (int i = 0; i < nv; ++i)
        pts->SetPoint(i, vb(0, i), vb(1, i), vb(2, i));

    auto poly = vtkSmartPointer<vtkPolyData>::New();
    poly->SetPoints(pts);

    if (mesh.containsElementNamed("normals")) {
        Rcpp::NumericMatrix nm = mesh["normals"];
        vtkNew<vtkFloatArray> norms;
        norms->SetName("Normals");
        norms->SetNumberOfComponents(3);
        norms->SetNumberOfTuples(nv);
        for (int i = 0; i < nv; ++i)
            norms->SetTuple3(i,
                static_cast<float>(nm(0, i)),
                static_cast<float>(nm(1, i)),
                static_cast<float>(nm(2, i)));
        poly->GetPointData()->SetNormals(norms);
    }
    return poly;
}

// vtkPolyData → mesh3d
Rcpp::List rvespa::vtk_to_mesh3d(vtkPolyData* poly) {
    const int nv = static_cast<int>(poly->GetNumberOfPoints());
    const int nf = static_cast<int>(poly->GetNumberOfCells());

    Rcpp::NumericMatrix vb(4, nv);
    for (int i = 0; i < nv; ++i) {
        double p[3];
        poly->GetPoint(i, p);
        vb(0, i) = p[0];
        vb(1, i) = p[1];
        vb(2, i) = p[2];
        vb(3, i) = 1.0;
    }

    Rcpp::IntegerMatrix it(3, nf);
    vtkNew<vtkIdList> idList;
    for (int j = 0; j < nf; ++j) {
        poly->GetCellPoints(j, idList);
        it(0, j) = static_cast<int>(idList->GetId(0)) + 1;
        it(1, j) = static_cast<int>(idList->GetId(1)) + 1;
        it(2, j) = static_cast<int>(idList->GetId(2)) + 1;
    }

    Rcpp::List result = Rcpp::List::create(
        Rcpp::Named("vb") = vb,
        Rcpp::Named("it") = it
    );
    result.attr("class") = Rcpp::CharacterVector::create("mesh3d", "shape3d");
    return result;
}

// vtkPolyData (point cloud) → mesh3d with empty $it and optional $normals (3×N)
Rcpp::List rvespa::vtk_to_pointcloud(vtkPolyData* poly) {
    const int nv = static_cast<int>(poly->GetNumberOfPoints());

    Rcpp::NumericMatrix vb(4, nv);
    for (int i = 0; i < nv; ++i) {
        double p[3];
        poly->GetPoint(i, p);
        vb(0, i) = p[0];
        vb(1, i) = p[1];
        vb(2, i) = p[2];
        vb(3, i) = 1.0;
    }

    Rcpp::List result = Rcpp::List::create(
        Rcpp::Named("vb") = vb,
        Rcpp::Named("it") = Rcpp::IntegerMatrix(3, 0)
    );

    vtkDataArray* normals = poly->GetPointData()->GetNormals();
    if (normals) {
        Rcpp::NumericMatrix nm(3, nv);
        for (int i = 0; i < nv; ++i) {
            double n[3];
            normals->GetTuple(i, n);
            nm(0, i) = n[0];
            nm(1, i) = n[1];
            nm(2, i) = n[2];
        }
        result["normals"] = nm;
    }

    result.attr("class") = Rcpp::CharacterVector::create("mesh3d", "shape3d");
    return result;
}

// vtkImageData → R list(dims, spacing, origin, values)
Rcpp::List rvespa::vtk_imagedata_to_r(vtkImageData* img) {
    int dims[3];
    img->GetDimensions(dims);

    double sp[3];
    img->GetSpacing(sp);

    double org[3];
    img->GetOrigin(org);

    vtkDataArray* scalars = img->GetPointData()->GetScalars();
    if (!scalars)
        Rcpp::stop("vtkImageData has no scalar array");

    const vtkIdType n = scalars->GetNumberOfTuples();
    Rcpp::NumericVector vals(n);
    for (vtkIdType i = 0; i < n; ++i)
        vals[i] = scalars->GetTuple1(i);

    return Rcpp::List::create(
        Rcpp::Named("dims")    = Rcpp::IntegerVector({dims[0], dims[1], dims[2]}),
        Rcpp::Named("spacing") = Rcpp::NumericVector({sp[0],   sp[1],   sp[2]}),
        Rcpp::Named("origin")  = Rcpp::NumericVector({org[0],  org[1],  org[2]}),
        Rcpp::Named("values")  = vals
    );
}

// vtkPolyData (2D triangles) → list of 4×2 closed ring matrices (for sf)
Rcpp::List rvespa::vtk_to_triangle_rings(vtkPolyData* poly) {
    const vtkIdType nf = poly->GetNumberOfCells();
    Rcpp::List rings(static_cast<int>(nf));
    vtkNew<vtkIdList> idList;

    for (vtkIdType j = 0; j < nf; ++j) {
        poly->GetCellPoints(j, idList);
        Rcpp::NumericMatrix ring(4, 2); // closed ring: 3 corners + repeat first
        for (int k = 0; k < 3; ++k) {
            double p[3];
            poly->GetPoint(idList->GetId(k), p);
            ring(k, 0) = p[0];
            ring(k, 1) = p[1];
        }
        ring(3, 0) = ring(0, 0);
        ring(3, 1) = ring(0, 1);
        rings[static_cast<int>(j)] = ring;
    }
    return rings;
}

// 1-based integer IDs → vtkSelection (0-based internally)
vtkSmartPointer<vtkSelection> rvespa::ids_to_vtk_selection(
    const Rcpp::IntegerVector& ids, bool field_is_point)
{
    vtkNew<vtkIdTypeArray> arr;
    arr->SetNumberOfTuples(ids.size());
    for (int i = 0; i < ids.size(); ++i)
        arr->SetValue(i, static_cast<vtkIdType>(ids[i] - 1));

    vtkNew<vtkSelectionNode> node;
    node->GetProperties()->Set(
        vtkSelectionNode::CONTENT_TYPE(), vtkSelectionNode::INDICES);
    node->GetProperties()->Set(
        vtkSelectionNode::FIELD_TYPE(),
        field_is_point ? vtkSelectionNode::POINT : vtkSelectionNode::CELL);
    node->SetSelectionList(arr);

    auto sel = vtkSmartPointer<vtkSelection>::New();
    sel->AddNode(node);
    return sel;
}

// VTK error observer callback
static void vtk_error_cb(vtkObject*, unsigned long, void* cd, void* calldata) {
    auto* ctx = static_cast<rvespa::VtkError*>(cd);
    ctx->occurred = true;
    if (calldata)
        ctx->message = static_cast<const char*>(calldata);
}

unsigned long rvespa::install_error_observer(vtkObject* obj, VtkError& ctx) {
    vtkNew<vtkCallbackCommand> cb;
    cb->SetCallback(vtk_error_cb);
    cb->SetClientData(&ctx);
    return obj->AddObserver(vtkCommand::ErrorEvent, cb);
}

void rvespa::check_vtk_error(const VtkError& err, const char* filter_name) {
    if (err.occurred)
        Rcpp::stop(std::string(filter_name) + ": " + err.message);
}

// VTK warning observer
static void vtk_warning_cb(vtkObject*, unsigned long, void* cd, void* calldata) {
    auto* ctx = static_cast<rvespa::VtkWarnings*>(cd);
    if (calldata)
        ctx->messages.push_back(static_cast<const char*>(calldata));
}

unsigned long rvespa::install_warning_observer(vtkObject* obj, VtkWarnings& ctx) {
    vtkNew<vtkCallbackCommand> cb;
    cb->SetCallback(vtk_warning_cb);
    cb->SetClientData(&ctx);
    return obj->AddObserver(vtkCommand::WarningEvent, cb);
}

void rvespa::emit_vtk_warnings(const VtkWarnings& w) {
    for (const auto& msg : w.messages)
        Rcpp::warning(msg);
}

// Round-trip test helper
// [[Rcpp::export]]
Rcpp::List rcpp_roundtrip(Rcpp::List mesh) {
    auto poly = rvespa::mesh3d_to_vtk(mesh);
    return rvespa::vtk_to_mesh3d(poly);
}
