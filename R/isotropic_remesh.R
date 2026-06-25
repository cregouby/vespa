#' Isotropic remeshing of a triangular surface mesh
#'
#' Remeshes a triangulated surface so that all edges approach a uniform target
#' length, using CGAL's isotropic remeshing algorithm.
#'
#' @param mesh A `mesh3d` object (from the rgl package).
#' @param target_length Target edge length for the result. A value of `-1`
#'   (default) lets Vespa choose automatically (1% of the bounding box
#'   diagonal).
#' @param protect_angle Feature edges whose dihedral angle exceeds this
#'   threshold (degrees) are preserved. Default `45`.
#' @param n_iterations Number of remeshing iterations. Default `1`.
#' @param update_attributes Whether to interpolate point and cell data arrays
#'   from the input mesh. Default `TRUE`.
#'
#' @return A `mesh3d` object with the remeshed surface.
#' @export
vespa_isotropic_remesh <- function(mesh,
                                   target_length     = -1.0,
                                   protect_angle     = 45.0,
                                   n_iterations      = 1L,
                                   update_attributes = TRUE) {
  .validate_mesh3d(mesh)
  rcpp_isotropic_remesh(
    mesh              = mesh,
    target_length     = as.double(target_length),
    protect_angle     = as.double(protect_angle),
    n_iterations      = as.integer(n_iterations),
    update_attributes = as.logical(update_attributes)
  )
}
