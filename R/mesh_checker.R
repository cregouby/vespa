#' Validate and optionally repair a triangular surface mesh
#'
#' Checks a mesh for watertightness and self-intersections using CGAL's
#' polygon mesh processing algorithms. Issues are reported via `cli` messages.
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
#'   TRUE` and repairs were applied.
#' @export
#' @examples
#' f <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh <- read_stl(f)
#' result <- mesh_check(mesh)
#' @family mesh_transforms
mesh_check <- function(
  mesh,
  check_watertight = TRUE,
  check_intersect = TRUE,
  attempt_repair = FALSE
) {
  .validate_mesh3d(mesh)

  vtk_msgs <- character(0)
  result <- withCallingHandlers(
    rcpp_mesh_check(
      mesh = mesh,
      check_watertight = as.logical(check_watertight),
      check_intersect = as.logical(check_intersect),
      attempt_repair = as.logical(attempt_repair)
    ),
    warning = function(w) {
      vtk_msgs <<- c(vtk_msgs, conditionMessage(w))
      invokeRestart("muffleWarning")
    }
  )

  for (msg in vtk_msgs) {
    # Strip VTK C++ prefix: "In File.cxx, line N\nClassName (0xADDR): "
    clean <- sub("^In [^\n]+\n[^:]*: ", "", msg)
    if (grepl("successful", clean, ignore.case = TRUE)) {
      cli::cli_inform(clean)
    } else {
      cli::cli_warn(clean)
    }
  }

  result
}
