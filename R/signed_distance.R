#' Compute a signed distance function (SDF) volume from a surface mesh
#'
#' Generates a 3-D signed distance field from a triangulated surface mesh using
#' CGAL's signed distance function. Negative values are inside the mesh, positive
#' values are outside.
#'
#' @param mesh A `mesh3d` object. The mesh should be closed and
#'   non-self-intersecting for physically meaningful signed distances.
#' @param base_resolution Base grid resolution along each axis. Default `64L`.
#' @param padding Number of voxels of padding to add around the mesh bounding
#'   box. Default `0L`.
#'
#' @return A list with elements:
#'   \describe{
#'     \item{`dims`}{Integer vector of length 3: grid dimensions (nx, ny, nz).}
#'     \item{`spacing`}{Numeric vector of length 3: voxel size in each direction.}
#'     \item{`origin`}{Numeric vector of length 3: world-space origin of the grid.}
#'     \item{`values`}{Numeric vector of length `prod(dims)`: signed distances in
#'       VTK (Fortran-order, x fastest) layout.}
#'     \item{`array`}{3-D array of shape `dims` built from `values`.}
#'   }
#' @export
vespa_sdf <- function(mesh,
                      base_resolution = 64L,
                      padding         = 0L) {
  cli::cli_abort(
    "vespa_sdf() requires {.code vtkCGALSignedDistanceFunction}, which is not available in this Vespa installation."
  )
}
