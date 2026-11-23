#include "converters.h"

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

#include <vtkCGALAlphaWrapping.h>
#include <vtkCGALBooleanOperation.h>
#include <vtkCGALIsotropicRemesher.h>
#include <vtkCGALMeshChecker.h>
#include <vtkCGALMeshDeformation.h>
#include <vtkCGALMeshSmoothing.h>
#include <vtkCGALMeshSubdivision.h>
#include <vtkCGALPatchFilling.h>
#include <vtkCGALRegionFairing.h>
#include <vtkCGALShapeSmoothing.h>
#include <vtkCGALSignedDistanceFunction.h>
#include <vtkCellArray.h>
#include <vtkIdTypeArray.h>
#include <vtkImageData.h>
#include <vtkNew.h>
#include <vtkPointData.h>

namespace {

struct Boundary {
    bool has_edges = false;
    bool triangular_cycle = false;
    vtkIdType a = 0;
    vtkIdType b = 0;
    vtkIdType c = 0;
};

Boundary inspect_boundary(vtkPolyData* poly) {
    Boundary result;
    if (!poly || poly->GetNumberOfPolys() == 0)
        return result;

    using Edge = std::pair<vtkIdType, vtkIdType>;
    std::map<Edge, unsigned int> edge_count;
    std::map<Edge, Edge> directed;

    vtkCellArray* cells = poly->GetPolys();
    cells->InitTraversal();
    vtkIdType npts = 0;
    const vtkIdType* pts = nullptr;
    while (cells->GetNextCell(npts, pts)) {
        for (vtkIdType i = 0; i < npts; ++i) {
            vtkIdType u = pts[i];
            vtkIdType v = pts[(i + 1) % npts];
            if (u == v)
                continue;
            Edge key = std::make_pair(std::min(u, v), std::max(u, v));
            ++edge_count[key];
            directed[key] = std::make_pair(u, v);
        }
    }

    std::map<vtkIdType, vtkIdType> next;
    for (const auto& entry : edge_count) {
        if (entry.second != 1)
            continue;
        result.has_edges = true;
        const Edge& edge = directed[entry.first];
        next[edge.first] = edge.second;
    }

    if (next.size() == 3) {
        vtkIdType a = next.begin()->first;
        vtkIdType b = next[a];
        vtkIdType c = next[b];
        if (a != b && b != c && c != a && next[c] == a) {
            result.triangular_cycle = true;
            result.a = a;
            result.b = b;
            result.c = c;
        }
    }

    return result;
}

void fill_triangular_hole(vtkPolyData* input, vtkPolyData* output,
                          const Boundary& boundary) {
    vtkNew<vtkPoints> points;
    points->DeepCopy(input->GetPoints());

    vtkNew<vtkCellArray> polys;
    polys->DeepCopy(input->GetPolys());
    vtkIdType triangle[3] = {boundary.b, boundary.a, boundary.c};
    polys->InsertNextCell(3, triangle);

    output->SetPoints(points);
    output->SetPolys(polys);
}

} // namespace

// [[Rcpp::export]]
Rcpp::List rcpp_alpha_wrap(
    Rcpp::List mesh,
    double alpha               = 5.0,
    double offset              = 3.0,
    bool   absolute_thresholds = false,
    bool   update_attributes   = true)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    vtkNew<vtkCGALAlphaWrapping> filter;
    filter->SetInputData(input);
    filter->SetAlpha(alpha);
    filter->SetOffset(offset);
    filter->SetAbsoluteThresholds(absolute_thresholds);
    filter->SetUpdateAttributes(update_attributes);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "AlphaWrapping");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("AlphaWrapping produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_mesh_smooth(
    Rcpp::List   mesh,
    unsigned int method            = 1,
    unsigned int n_iterations      = 10,
    bool         safety_constraints = false,
    bool         update_attributes  = true)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    vtkNew<vtkCGALMeshSmoothing> filter;
    filter->SetInputData(input);
    filter->SetSmoothingMethod(method);
    filter->SetNumberOfIterations(n_iterations);
    filter->SetUseSafetyConstraints(safety_constraints);
    filter->SetUpdateAttributes(update_attributes);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "MeshSmoothing");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("MeshSmoothing produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_subdivide(
    Rcpp::List   mesh,
    int          subdivision_type  = 3,
    unsigned int n_iterations      = 1,
    bool         update_attributes = true)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    vtkNew<vtkCGALMeshSubdivision> filter;
    filter->SetInputData(input);
    filter->SetSubdivisionType(subdivision_type);
    filter->SetNumberOfIterations(n_iterations);
    filter->SetUpdateAttributes(update_attributes);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "MeshSubdivision");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("MeshSubdivision produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_shape_smooth(
    Rcpp::List   mesh,
    unsigned int n_iterations      = 1,
    double       time_step         = 1e-4,
    bool         update_attributes = true)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    vtkNew<vtkCGALShapeSmoothing> filter;
    filter->SetInputData(input);
    filter->SetNumberOfIterations(n_iterations);
    filter->SetTimeStep(time_step);
    filter->SetUpdateAttributes(update_attributes);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "ShapeSmoothing");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("ShapeSmoothing produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_mesh_check(
    Rcpp::List mesh,
    bool check_watertight = true,
    bool check_intersect  = true,
    bool attempt_repair   = false)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    vtkNew<vtkCGALMeshChecker> filter;
    filter->SetInputData(input);
    filter->SetCheckWatertight(check_watertight);
    filter->SetCheckIntersect(check_intersect);
    filter->SetAttemptRepair(attempt_repair);

    vespa::VtkError       err;
    vespa::VtkWarnings    warn;
    vespa::VtkQuietOutput silence;   // suppresses vtkExecutive stderr noise
    vespa::install_error_observer(filter, err);
    vespa::install_warning_observer(filter, warn);
    filter->Update();
    vespa::emit_vtk_warnings(warn);
    vespa::check_vtk_error(err, "MeshChecker");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("MeshChecker produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_isotropic_remesh(
    Rcpp::List mesh,
    double       target_length      = -1.0,
    double       protect_angle      = 45.0,
    int          n_iterations       = 1,
    bool         update_attributes  = true)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    vtkNew<vtkCGALIsotropicRemesher> filter;
    filter->SetInputData(input);
    filter->SetTargetLength(target_length);
    filter->SetProtectAngle(protect_angle);
    filter->SetNumberOfIterations(n_iterations);
    filter->SetUpdateAttributes(update_attributes);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "IsotropicRemesher");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("IsotropicRemesher produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_boolean_op(
    Rcpp::List mesh_a,
    Rcpp::List mesh_b,
    int  operation        = 0,
    bool update_attributes = true)
{
    auto inputA = vespa::mesh3d_to_vtk(mesh_a);
    auto inputB = vespa::mesh3d_to_vtk(mesh_b);

    vtkNew<vtkCGALBooleanOperation> filter;
    filter->SetInputData(0, inputA);
    filter->SetInputData(1, inputB);
    filter->SetOperationType(operation);
    filter->SetUpdateAttributes(update_attributes);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "BooleanOperation");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("BooleanOperation produced empty output. "
                   "Both meshes must be closed and non-self-intersecting.");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_patch_fill(
    Rcpp::List          mesh,
    Rcpp::IntegerVector point_ids,
    int                 fairing_continuity = 1,
    bool                update_attributes  = true)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    if (point_ids.size() == 0) {
        Boundary boundary = inspect_boundary(input);
        if (!boundary.has_edges)
            return vespa::vtk_to_mesh3d(input);
        if (boundary.triangular_cycle) {
            vtkNew<vtkPolyData> filled;
            fill_triangular_hole(input, filled, boundary);
            return vespa::vtk_to_mesh3d(filled);
        }
    }

    vtkNew<vtkCGALPatchFilling> filter;
    filter->SetInputData(0, input);

    if (point_ids.size() > 0) {
        auto sel = vespa::ids_to_vtk_selection(point_ids);
        filter->SetInputData(1, sel);
    }

    filter->SetFairingContinuity(fairing_continuity);
    filter->SetUpdateAttributes(update_attributes);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "PatchFilling");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("PatchFilling produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_region_fair(
    Rcpp::List          mesh,
    Rcpp::IntegerVector point_ids,
    bool                update_attributes = true)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    auto sel = vespa::ids_to_vtk_selection(point_ids);

    vtkNew<vtkCGALRegionFairing> filter;
    filter->SetInputData(0, input);
    filter->SetInputData(1, sel);
    filter->SetUpdateAttributes(update_attributes);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "RegionFairing");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("RegionFairing produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_mesh_deform(
    Rcpp::List          mesh,
    Rcpp::IntegerVector control_ids,
    Rcpp::NumericMatrix target_coords,
    Rcpp::IntegerVector roi_ids,
    int          mode         = 0,
    double       sre_alpha    = 0.02,
    unsigned int n_iterations = 5,
    double       tolerance    = 1e-4)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    // Build control-point vtkPolyData for port 1
    const int n_ctrl = control_ids.size();
    vtkNew<vtkPoints> ctrl_pts;
    ctrl_pts->SetNumberOfPoints(n_ctrl);
    for (int i = 0; i < n_ctrl; ++i)
        ctrl_pts->SetPoint(i, target_coords(i, 0), target_coords(i, 1), target_coords(i, 2));

    vtkNew<vtkIdTypeArray> gids;
    gids->SetName("GlobalNodeIds");
    gids->SetNumberOfTuples(n_ctrl);
    for (int i = 0; i < n_ctrl; ++i)
        gids->SetValue(i, static_cast<vtkIdType>(control_ids[i] - 1)); // 1→0-based

    vtkNew<vtkPolyData> ctrl_poly;
    ctrl_poly->SetPoints(ctrl_pts);
    ctrl_poly->GetPointData()->SetGlobalIds(gids);

    vtkNew<vtkCGALMeshDeformation> filter;
    filter->SetInputData(0, input);
    filter->SetInputData(1, ctrl_poly);
    filter->SetMode(mode);
    filter->SetSreAlpha(sre_alpha);
    filter->SetNumberOfIterations(n_iterations);
    filter->SetTolerance(tolerance);
    filter->SetGlobalIdArray("GlobalNodeIds");

    if (roi_ids.size() > 0) {
        auto sel = vespa::ids_to_vtk_selection(roi_ids);
        filter->SetInputData(2, sel);
    }

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "MeshDeformation");

    vtkPolyData* out = filter->GetOutput();
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("MeshDeformation produced empty output");

    return vespa::vtk_to_mesh3d(out);
}

// [[Rcpp::export]]
Rcpp::List rcpp_sdf(
    Rcpp::List   mesh,
    unsigned int base_resolution = 64,
    int          padding         = 0)
{
    auto input = vespa::mesh3d_to_vtk(mesh);

    vtkNew<vtkCGALSignedDistanceFunction> filter;
    filter->SetInputData(input);
    filter->SetBaseResolution(base_resolution);
    filter->SetPadding(padding);

    vespa::VtkError err;
    vespa::install_error_observer(filter, err);
    filter->Update();
    vespa::check_vtk_error(err, "SignedDistanceFunction");

    vtkImageData* out = vtkImageData::SafeDownCast(filter->GetOutput());
    if (!out || out->GetNumberOfPoints() == 0)
        Rcpp::stop("SignedDistanceFunction produced empty output");

    return vespa::vtk_imagedata_to_r(out);
}
