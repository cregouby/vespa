#' Read a point cloud file
#'
#' Reads a point cloud from disk using CGAL's point set reader. Supported
#' formats: `.las`, `.off`, `.ply`, `.xyz`.
#'
#' @param filename Path to the point cloud file. The file must exist and be
#'   readable.
#'
#' @return A `mesh3d` object representing the point cloud. `$vb` is a 4×N
#'   matrix of homogeneous point coordinates; `$it` is an empty 3×0 matrix.
#'   If the file contains normal vectors, they are returned in `$normals`
#'   (3×N matrix).
#' @export
vespa_read_points <- function(filename) {
  filename <- as.character(filename)
  if (!file.exists(filename))
    cli::cli_abort("File not found: {.path {filename}}")
  rcpp_read_points(filename)
}
