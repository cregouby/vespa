#' Extract an isosurface from a 3-D scalar field
#'
#' Generic function that extracts an isosurface from a volumetric scalar
#' field. Currently implemented for objects of class [`sdf_volume`]
#' (returned by [signed_distance_function()]).
#'
#' @param x An object from which to extract an isosurface.
#' @param isovalue Numeric. The iso-level at which to extract the surface.
#' @param ... Additional arguments passed to methods.
#'
#' @return A `mesh3d` object (from package **rgl**).
#'
#' @seealso [signed_distance_function()] to build an `sdf_volume`.
#' @export
extract_isosurface <- function(x, isovalue = 0, ...) {
  UseMethod("extract_isosurface")
}

#' @rdname extract_isosurface
#' @export
extract_isosurface.sdf_volume <- function(x, isovalue = 0, ...) {
  result <- extract_isosurface_cpp(
    array     = as.vector(x$array),
    dims      = as.integer(x$dims),
    spacing   = as.numeric(x$spacing),
    origin    = as.numeric(x$origin),
    isovalue  = as.numeric(isovalue)
  )
  
  rgl::mesh3d(
    vertices  = result$vb,
    triangles = result$ib
  )
}

#' @rdname extract_isosurface
#' @export
extract_isosurface.default <- function(x, isovalue = 0, ...) {
  # Give a helpful, targeted error message
  if (inherits(x, "mesh3d")) {
    cli::cli_abort(
      "`extract_isosurface()` expects a volumetric field (class `sdf_volume`), ",
      "not a surface mesh.\n",
      "Did you mean to call `signed_distance_function(mesh)` first?",
      call. = FALSE
    )
  }
  cli::cli_abort(
    "{.fn extract_isosurface()} has no method for objects of class {.class {class(x)}}",
    call. = FALSE
  )
}

#' SDF volume objects
#'
#' An `sdf_volume` is a 3-D scalar field defined on a regular grid.
#' It is typically produced by [signed_distance_function()], where
#' negative values lie inside the source mesh and positive values outside.
#'
#' @section Components:
#' \describe{
#'   \item{`dims`}{Integer vector of length 3: grid dimensions `(nx, ny, nz)`.}
#'   \item{`spacing`}{Numeric vector of length 3: voxel size along each axis.}
#'   \item{`origin`}{Numeric vector of length 3: world-space origin of the grid.}
#'   \item{`array`}{3-D numeric array of signed distances (x fastest).}
#' }
#'
#' @seealso [signed_distance_function()], [extract_isosurface()]
#' @name sdf_volume
#' @aliases sdf_volume-class
NULL

#' @export
print.sdf_volume <- function(x, ...) {
  cat("<sdf_volume>\n")
  cat(sprintf("  dims:    %d x %d x %d  (%s voxels)\n",
              x$dims[1], x$dims[2], x$dims[3],
              format(prod(x$dims), big.mark = ",")))
  cat(sprintf("  spacing: %.4g x %.4g x %.4g\n",
              x$spacing[1], x$spacing[2], x$spacing[3]))
  cat(sprintf("  origin:  (%.4g, %.4g, %.4g)\n",
              x$origin[1], x$origin[2], x$origin[3]))
  rng <- range(x$array, na.rm = TRUE)
  cat(sprintf("  values:  [%.4g, %.4g]\n", rng[1], rng[2]))
  invisible(x)
}

#' @export
plot.sdf_volume <- function(x, isovalue = 0, color = "lightblue", ...) {
  if (!requireNamespace("rgl", quietly = TRUE)) {
    stop("Package 'rgl' is required to plot an sdf_volume.", call. = FALSE)
  }
  mesh <- extract_isosurface(x, isovalue = isovalue)
  rgl::open3d()
  rgl::shade3d(mesh, color = color, ...)
  rgl::title3d(main = sprintf("Isosurface at level = %g", isovalue))
  invisible(mesh)
}