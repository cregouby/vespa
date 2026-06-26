#' Estimate point cloud normals via PCA
#'
#' Estimates and orients surface normals for an unoriented point cloud using
#' CGAL's PCA-based normal estimation algorithm.
#'
#' @param mesh A `mesh3d` object representing a point cloud (may have an empty
#'   `$it` face matrix).
#' @param n_neighbors Number of nearest neighbours used for local PCA. Default
#'   `18L`.
#' @param orient Whether to orient the estimated normals consistently. Default
#'   `TRUE`.
#' @param delete_unoriented Whether to remove points whose normals could not be
#'   reliably oriented. Default `TRUE`.
#'
#' @return A `mesh3d` object identical to the input but with an additional
#'   `$normals` field (3×N numeric matrix, one column per point).
#' @seealso [poisson_reconstruction()]
#' @export
pca_estimate_normals <- function(mesh,
                              n_neighbors       = 18L,
                              orient            = TRUE,
                              delete_unoriented = TRUE) {
  .validate_mesh3d(mesh)
  rcpp_pca_normals(
    mesh              = mesh,
    n_neighbors       = as.integer(n_neighbors),
    orient            = as.logical(orient),
    delete_unoriented = as.logical(delete_unoriented)
  )
}
