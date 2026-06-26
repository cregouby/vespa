#' Generate a watertight mesh via alpha wrapping
#'
#' Constructs a 2-manifold, watertight surface mesh from a point cloud or
#' triangle soup using CGAL's alpha wrapping algorithm.
#'
#' @param mesh A `mesh3d` object (input point cloud or triangle soup).
#' @param alpha Maximum circumradius of output faces. Smaller values produce
#'   tighter wraps that capture finer concavities. Default `5` (interpreted as
#'   5% of the bounding-box diagonal when `absolute_thresholds = FALSE`).
#' @param offset Mesh dilatation amount. Default `3`.
#' @param absolute_thresholds If `TRUE`, `alpha` and `offset` are in the same
#'   units as the mesh coordinates. If `FALSE` (default), they are percentages
#'   of the bounding-box diagonal.
#' @param update_attributes Whether to interpolate point and cell data from the
#'   input. Default `TRUE`.
#'
#' @return A `mesh3d` object representing the wrapped surface.
#' @export
alpha_wrapping <- function(mesh,
                             alpha               = 5.0,
                             offset              = 3.0,
                             absolute_thresholds = FALSE,
                             update_attributes   = TRUE) {
  .validate_mesh3d(mesh)
  rcpp_alpha_wrap(
    mesh                = mesh,
    alpha               = as.double(alpha),
    offset              = as.double(offset),
    absolute_thresholds = as.logical(absolute_thresholds),
    update_attributes   = as.logical(update_attributes)
  )
}
