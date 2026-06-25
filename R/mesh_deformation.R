#' Deform a triangular surface mesh
#'
#' Deforms a triangulated surface mesh by moving a set of control vertices to
#' specified target positions, propagating the deformation across the surface
#' using CGAL's as-rigid-as-possible (ARAP) or smoothed rotation enhanced ARAP
#' (SRE-ARAP) algorithms.
#'
#' @param mesh A `mesh3d` object.
#' @param control_ids Integer vector of 1-based vertex indices that act as
#'   handles.
#' @param target_coords Numeric matrix with `length(control_ids)` rows and 3
#'   columns (x, y, z). Target world-space positions for each handle.
#' @param roi_ids Integer vector of 1-based vertex indices forming the region of
#'   influence. Use `integer(0)` (the default) to let the filter determine the
#'   region automatically.
#' @param mode Deformation algorithm: `"smooth"` (ARAP, default) or
#'   `"sre_arap"` (Smoothed Rotation Enhanced ARAP).
#' @param sre_alpha Blending weight for SRE-ARAP (0 = pure ARAP,
#'   1 = pure shape). Default `0.02`.
#' @param n_iterations Number of optimisation iterations. Default `5L`.
#' @param tolerance Convergence tolerance. Default `1e-4`.
#'
#' @return A `mesh3d` object with the deformed surface.
#' @export
vespa_mesh_deform <- function(mesh,
                              control_ids,
                              target_coords,
                              roi_ids      = integer(0),
                              mode         = c("smooth", "sre_arap"),
                              sre_alpha    = 0.02,
                              n_iterations = 5L,
                              tolerance    = 1e-4) {
  .validate_mesh3d(mesh)
  if (missing(control_ids) || length(control_ids) == 0L)
    cli::cli_abort("{.arg control_ids} must be a non-empty integer vector of vertex indices")
  target_coords <- as.matrix(target_coords)
  if (!is.numeric(target_coords) || ncol(target_coords) != 3L)
    cli::cli_abort("{.arg target_coords} must be a numeric matrix with 3 columns (x, y, z)")
  if (nrow(target_coords) != length(control_ids))
    cli::cli_abort("{.arg target_coords} must have one row per entry in {.arg control_ids}")
  mode_code <- switch(match.arg(mode),
    smooth   = 0L,
    sre_arap = 1L
  )
  rcpp_mesh_deform(
    mesh          = mesh,
    control_ids   = as.integer(control_ids),
    target_coords = target_coords,
    roi_ids       = as.integer(roi_ids),
    mode          = mode_code,
    sre_alpha     = as.double(sre_alpha),
    n_iterations  = as.integer(n_iterations),
    tolerance     = as.double(tolerance)
  )
}
