#' Fill holes in a triangular surface mesh
#'
#' Detects and fills holes in a triangulated surface mesh using CGAL's hole
#' filling algorithm. When `point_ids` is supplied, only the hole whose boundary
#' passes through those points is filled; otherwise all detected holes are
#' filled.
#'
#' @param mesh A `mesh3d` object.
#' @param point_ids Integer vector of 1-based vertex indices on the hole
#'   boundary to fill. Use `integer(0)` (the default) to fill all holes.
#' @param fairing_continuity Integer controlling the continuity order of the
#'   filled patch: `0` (C0), `1` (C1, default), or `2` (C2).
#' @param update_attributes Whether to interpolate point and cell data from the
#'   input. Default `TRUE`.
#'
#' @return A `mesh3d` object with the hole(s) filled.
#' @export
patch_filling <- function(mesh,
                             point_ids          = integer(0),
                             fairing_continuity = 1L,
                             update_attributes  = TRUE) {
  .validate_mesh3d(mesh)
  rcpp_patch_fill(
    mesh               = mesh,
    point_ids          = as.integer(point_ids),
    fairing_continuity = as.integer(fairing_continuity),
    update_attributes  = as.logical(update_attributes)
  )
}
