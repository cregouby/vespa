#' Boolean operation on two triangular surface meshes
#'
#' Computes the union, intersection, or difference of two closed triangulated
#' surface meshes using CGAL's polygon mesh processing boolean operations.
#' Both meshes must be closed (watertight) and free of self-intersections.
#'
#' @param mesh_a A `mesh3d` object (first operand).
#' @param mesh_b A `mesh3d` object (second operand).
#' @param operation One of `"difference"` (default), `"intersection"`, or
#'   `"union"`. The difference is `mesh_a` minus `mesh_b`.
#' @param update_attributes Whether to interpolate point and cell data from the
#'   inputs. Default `TRUE`.
#'
#' @return A `mesh3d` object representing the result of the boolean operation.
#' @export
#' @examples
#' \donttest{
#' f <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh  <- read_stl(f)
#' mesh2 <- mesh
#' mesh2$vb[1, ] <- mesh2$vb[1, ] + 0.5  # shift copy along X
#' result <- boolean_operation(mesh, mesh2, operation = "union")
#' }
boolean_operation <- function(mesh_a,
                             mesh_b,
                             operation         = c("difference", "intersection", "union"),
                             update_attributes = TRUE) {
  .validate_mesh3d(mesh_a, arg = "mesh_a")
  .validate_mesh3d(mesh_b, arg = "mesh_b")
  op_code <- switch(match.arg(operation),
    difference   = 0L,
    intersection = 1L,
    union        = 2L
  )
  rcpp_boolean_op(
    mesh_a            = mesh_a,
    mesh_b            = mesh_b,
    operation         = op_code,
    update_attributes = as.logical(update_attributes)
  )
}
