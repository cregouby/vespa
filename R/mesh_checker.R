#' Validate and optionally repair a triangular surface mesh
#'
#' Checks a mesh for watertightness and self-intersections using CGAL's
#' polygon mesh processing algorithms. Issues are reported as R warnings.
#' When `attempt_repair = TRUE`, Vespa attempts to fix detected problems and
#' returns the repaired mesh.
#'
#' @param mesh A `mesh3d` object.
#' @param check_watertight Whether to check if the mesh is closed and bounds a
#'   volume. Default `TRUE`.
#' @param check_intersect Whether to check for self-intersections. Default
#'   `TRUE`.
#' @param attempt_repair If `TRUE`, Vespa attempts to fix detected issues
#'   (fill holes, re-orient faces, remove self-intersections). Default `FALSE`.
#'
#' @return A `mesh3d` object — identical to the input unless `attempt_repair =
#'   TRUE` and repairs were applied. R warnings are emitted for each issue
#'   detected.
#' @export
vespa_mesh_check <- function(mesh,
                             check_watertight = TRUE,
                             check_intersect  = TRUE,
                             attempt_repair   = FALSE) {
  .validate_mesh3d(mesh)
  rcpp_mesh_check(
    mesh             = mesh,
    check_watertight = as.logical(check_watertight),
    check_intersect  = as.logical(check_intersect),
    attempt_repair   = as.logical(attempt_repair)
  )
}
