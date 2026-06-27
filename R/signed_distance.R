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
#' @return An object of class `sdf_volume` (a list subclass) with elements:
#'   \describe{
#'     \item{`dims`}{Integer vector of length 3: grid dimensions (nx, ny, nz).}
#'     \item{`spacing`}{Numeric vector of length 3: voxel size in each direction.}
#'     \item{`origin`}{Numeric vector of length 3: world-space origin of the grid.}
#'     \item{`array`}{3-D numeric array of shape `dims` (x fastest, VTK/Fortran order)
#'       containing the signed distances. Use `as.vector(sdf$array)` to recover
#'       a flat vector.}
#'   }
#'   This object is accepted by [extract_isosurface()] and has dedicated
#'   `print()` and `plot()` methods.
#'
#' @seealso [extract_isosurface()] to extract a `mesh3d` from an `sdf_volume`.
#'
#' @export
#' @examples
#' \donttest{
#' f <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh <- read_stl(f)
#' sdf  <- signed_distance_function(mesh, base_resolution = 32L)
#' sdf                              # uses print.sdf_volume
#' range(sdf$array)                 # negative inside, positive outside
#' }
signed_distance_function <- function(mesh,
                                     base_resolution = 64L,
                                     padding         = 0L) {
  
  if (!inherits(mesh, "mesh3d")) {
    cli::cli_abort("{.arg mesh} must be a {.cls mesh3d} object, not {.obj_type_friendly {mesh}}.")
  }
  
  res <- rcpp_sdf(mesh,
                  base_resolution = as.integer(base_resolution),
                  padding         = as.integer(padding))
  
  # Reshape flat vector into a 3-D array and rename the field
  dim(res[["values"]]) <- res$dims
  names(res)[match("values", names(res))] <- "array"
  
  # Attach the S3 class *before* printing so that print.sdf_volume is used
  # if the user inspects the object immediately
  class(res) <- c("sdf_volume", "list")
  
  # Informational message 
  nvox      <- prod(res$dims)
  footprint <- format(structure(nvox * 8L, class = "object_size"), units = "auto")
  cli::cli_inform("SDF grid: {res$dims[1]} \u00d7 {res$dims[2]} \u00d7 {res$dims[3]} \u2014 {format(nvox, big.mark = ',')} voxels, ~{footprint}")
  
  res
}