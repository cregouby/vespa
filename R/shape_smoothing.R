#' Shape-preserving smoothing of a triangular surface mesh
#'
#' Smooths a triangulated surface while preserving its overall shape, using
#' CGAL's mean curvature flow algorithm.
#'
#' @param mesh A `mesh3d` object.
#' @param n_iterations Number of smoothing iterations. Default `1L`.
#' @param time_step Controls the smoothing speed. Smaller values give a more
#'   conservative smoothing. Default `1e-4`.
#' @param update_attributes Whether to interpolate point and cell data from the
#'   input. Default `TRUE`.
#'
#' @return A `mesh3d` object with the smoothed surface.
#' @export
#' @examples
#' \donttest{
#' f <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh <- read_stl(f)
#' result <- shape_smoothing(mesh, n_iterations = 2L)
#' }
shape_smoothing <- function(mesh,
                               n_iterations      = 1L,
                               time_step         = 1e-4,
                               update_attributes = TRUE) {
  .validate_mesh3d(mesh)
  rcpp_shape_smooth(
    mesh              = mesh,
    n_iterations      = as.integer(n_iterations),
    time_step         = as.double(time_step),
    update_attributes = as.logical(update_attributes)
  )
}
