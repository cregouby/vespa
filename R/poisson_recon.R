#' Reconstruct a surface from an oriented point cloud (Poisson)
#'
#' Reconstructs a closed triangulated surface from an oriented point cloud using
#' CGAL's Poisson surface reconstruction algorithm. The input point cloud must
#' carry estimated normals (e.g., from [vespa_pca_normals()]).
#'
#' @param mesh A `mesh3d` object representing a point cloud. Must contain a
#'   `$normals` field (3×N matrix) produced by [vespa_pca_normals()].
#' @param min_angle Minimum triangle angle in the output mesh (degrees). Default
#'   `20`.
#' @param max_size Maximum triangle size relative to the average point spacing.
#'   Default `2`.
#' @param distance Maximum distance from the reconstructed surface to the input
#'   points, relative to average spacing. Default `0.375`.
#' @param gen_normals Whether to generate normals on the output surface. Default
#'   `TRUE`.
#'
#' @return A `mesh3d` object with the reconstructed surface.
#' @seealso [vespa_pca_normals()], [vespa_advancing_front()]
#' @export
vespa_poisson_recon <- function(mesh,
                                min_angle   = 20,
                                max_size    = 2,
                                distance    = 0.375,
                                gen_normals = TRUE) {
  .validate_mesh3d(mesh)
  if (is.null(mesh$normals))
    cli::cli_abort(
      c("{.arg mesh} must have a {.field $normals} field.",
        "i" = "Run {.fn vespa_pca_normals} first to estimate normals.")
    )
  rcpp_poisson_recon(
    mesh        = mesh,
    min_angle   = as.double(min_angle),
    max_size    = as.double(max_size),
    distance    = as.double(distance),
    gen_normals = as.logical(gen_normals)
  )
}
