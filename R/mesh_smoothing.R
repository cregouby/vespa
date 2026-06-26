#' Smooth a triangular surface mesh
#'
#' Smooths a triangulated surface using CGAL's polygon mesh processing
#' smoothing algorithms.
#'
#' @param mesh A `mesh3d` object.
#' @param method Smoothing method: `"tangential"` (default) for tangential
#'   relaxation, or `"angle_area"` for angle and area regularisation.
#' @param n_iterations Number of smoothing iterations. Default `10L`.
#' @param safety_constraints If `TRUE`, applies safety constraints to limit
#'   vertex displacement. Default `FALSE`.
#' @param update_attributes Whether to interpolate point and cell data from the
#'   input. Default `TRUE`.
#'
#' @return A `mesh3d` object with the smoothed surface.
#' @export
#' @examples
#' \donttest{
#' f <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh <- read_stl(f)
#' smooth <- mesh_smoothing(mesh, method = "tangential", n_iterations = 3L)
#' }
mesh_smoothing <- function(mesh,
                              method             = c("tangential", "angle_area"),
                              n_iterations       = 10L,
                              safety_constraints = FALSE,
                              update_attributes  = TRUE) {
  .validate_mesh3d(mesh)
  method_code <- switch(match.arg(method),
    tangential = 1L,
    angle_area = 2L
  )
  rcpp_mesh_smooth(
    mesh             = mesh,
    method           = method_code,
    n_iterations     = as.integer(n_iterations),
    safety_constraints = as.logical(safety_constraints),
    update_attributes = as.logical(update_attributes)
  )
}
