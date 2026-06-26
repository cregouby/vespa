#' Read a surface mesh or point cloud from a VTP file
#'
#' Reads a `vtkPolyData` object from a VTK XML PolyData (`.vtp`) file using
#' VTK's XML reader. Both ASCII and binary (including binary-appended) formats
#' are supported.
#'
#' @param filename Path to the `.vtp` file. The file must exist and be readable.
#'
#' @return A `mesh3d` object. `$vb` is a 4×N matrix of homogeneous vertex
#'   coordinates; `$it` is a 3×M matrix of 1-based triangle indices.
#'   For point-cloud VTP files (no polygon cells), `$it` is a 3×0 empty
#'   matrix.
#' @export
#' @examples
#' \donttest{
#' url <- paste0(
#'   "https://gitlab.kitware.com/vtk/meshing/vespa/-/raw/master/",
#'   "Data/Testing/hand.vtp?ref_type=heads"
#' )
#' tmp <- tempfile(fileext = ".vtp")
#' download.file(url, tmp, quiet = TRUE)
#' mesh <- read_vtp(tmp)
#' print(mesh)
#' }
read_vtp <- function(filename) {
  filename <- as.character(filename)
  if (!file.exists(filename))
    cli::cli_abort("File not found: {.path {filename}}")
  rcpp_read_vtp(filename)
}
