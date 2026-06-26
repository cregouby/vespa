#' Reconstruct a surface from a point cloud (Advancing Front)
#'
#' Reconstructs a triangulated surface from an unoriented point cloud using
#' CGAL's advancing front surface reconstruction algorithm. Unlike Poisson
#' reconstruction, no normals are required.
#'
#' @param mesh A `mesh3d` object representing a point cloud (may have an empty
#'   `$it` face matrix).
#' @param per Priority function parameter controlling the trade-off between
#'   advancing front size and point proximity. Default `0` (automatic).
#' @param radius_ratio_bound Maximum radius ratio of generated triangles
#'   (quality bound). Default `5`.
#'
#' @return A `mesh3d` object with the reconstructed surface.
#' @seealso [poisson_reconstruction()]
#' @export
#' @examples
#' \donttest{
#' f     <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh  <- read_stl(f)
#' cloud <- mesh; cloud$it <- matrix(integer(0), 3L, 0L)
#' result <- advancing_front_reconstruction(cloud)
#' }
advancing_front_reconstruction <- function(mesh,
                                  per               = 0,
                                  radius_ratio_bound = 5) {
  .validate_mesh3d(mesh)
  rcpp_advancing_front(
    mesh               = mesh,
    per                = as.double(per),
    radius_ratio_bound = as.double(radius_ratio_bound)
  )
}
