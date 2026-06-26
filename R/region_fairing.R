#' Fair a region of a triangular surface mesh
#'
#' Smooths a user-defined region of a triangulated surface mesh using CGAL's
#' region fairing algorithm. The region vertices are moved to minimise a
#' local fairing energy while the boundary of the region is kept fixed.
#'
#' @param mesh A `mesh3d` object.
#' @param point_ids Integer vector of 1-based vertex indices defining the region
#'   to be faired.
#' @param update_attributes Whether to interpolate point and cell data from the
#'   input. Default `TRUE`.
#'
#' @return A `mesh3d` object with the selected region faired.
#' @export
region_fairing <- function(mesh,
                              point_ids,
                              update_attributes = TRUE) {
  .validate_mesh3d(mesh)
  if (missing(point_ids) || length(point_ids) == 0L)
    cli::cli_abort("{.arg point_ids} must be a non-empty integer vector of vertex indices")
  rcpp_region_fair(
    mesh              = mesh,
    point_ids         = as.integer(point_ids),
    update_attributes = as.logical(update_attributes)
  )
}
