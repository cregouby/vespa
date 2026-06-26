#' Read a surface mesh from an STL file
#'
#' Reads a triangulated surface mesh from an ASCII or binary STL file using
#' VTK's STL reader.
#'
#' @param filename Path to the STL file. The file must exist and be readable.
#'
#' @return A `mesh3d` object. `$vb` is a 4×N matrix of homogeneous vertex
#'   coordinates; `$it` is a 3×M matrix of 1-based triangle indices.
#' @export
#' @examples
#' f <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh <- read_stl(f)
#' print(mesh)
read_stl <- function(filename) {
  filename <- as.character(filename)
  if (!file.exists(filename))
    cli::cli_abort("File not found: {.path {filename}}")
  rcpp_read_stl(filename)
}
