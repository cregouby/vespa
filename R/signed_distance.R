#' Compute a signed distance function (SDF) volume from a surface mesh
#'
#' Generates a 3-D signed distance field from a triangulated surface mesh using
#' CGAL's signed distance function. Negative values are inside the mesh, positive
#' values are outside.
#'
#' @param mesh A `mesh3d` object. The mesh should be closed and
#'   non-self-intersecting for physically meaningful signed distances.
#' @param base_resolution Number of grid nodes along the shortest bounding-box
#'   axis. Default `64L`.
#' @param padding Number of voxels of padding added on each side of each axis
#'   around the mesh bounding box. Default `0L`.
#'
#' @note
#' **Grid resolution and cubic voxels.**
#' Let \eqn{L_{\min} = \min(L_x, L_y, L_z)} be the shortest bounding-box
#' extent. The voxel size is
#' \deqn{h = \frac{L_{\min}}{b - 1},}
#' where \eqn{b} is `base_resolution`. The number of grid nodes along each
#' axis is then
#' \deqn{N_{\alpha} = \Bigl\lfloor \frac{L_{\alpha}}{h} \Bigr\rceil + 1 + 2\,p,}
#' where \eqn{\lfloor\cdot\rceil} denotes rounding to the nearest integer and
#' \eqn{p} is `padding`. This guarantees that voxels are cubic regardless of
#' the mesh aspect ratio.
#'
#' @return A list with elements:
#'   \describe{
#'     \item{`dims`}{Integer vector of length 3: grid dimensions (nx, ny, nz).}
#'     \item{`spacing`}{Numeric vector of length 3: voxel size in each direction.}
#'     \item{`origin`}{Numeric vector of length 3: world-space origin of the grid.}
#'     \item{`array`}{3-D numeric array of shape `dims` (x fastest, VTK/Fortran order)
#'       containing the signed distances. Use `as.vector(sdf$array)` to recover
#'       a flat vector.}
#'   }
#' @export
#' @examples
#' \donttest{
#' f <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh <- read_stl(f)
#' sdf  <- signed_distance_function(mesh, base_resolution = 32L)
#' range(sdf$array)   # negative inside, positive outside
#' }
signed_distance_function <- function(mesh,
                      base_resolution = 64L,
                      padding         = 0L) {
  res <- rcpp_sdf(mesh,
                  base_resolution = as.integer(base_resolution),
                  padding         = as.integer(padding))
  dim(res[["values"]]) <- res$dims
  names(res)[match("values", names(res))] <- "array"
  nvox      <- prod(res$dims)
  footprint <- format(structure(nvox * 8L, class = "object_size"), units = "auto")
  cli::cli_inform("SDF grid: {res$dims[1]} × {res$dims[2]} × {res$dims[3]} — {format(nvox, big.mark = ',')} voxels, ~{footprint}")
  res
}
