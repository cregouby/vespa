#' Subdivide a triangular surface mesh
#'
#' Applies a subdivision scheme to a triangulated surface mesh using CGAL's
#' polygon mesh processing subdivision algorithms.
#'
#' @param mesh A `mesh3d` object.
#' @param type Subdivision scheme: `"sqrt3"` (default), `"loop"`,
#'   `"catmull_clark"`, or `"doo_sabin"`.
#' @param n_iterations Number of subdivision iterations. Default `1L`.
#' @param update_attributes Whether to interpolate point and cell data from the
#'   input. Default `TRUE`.
#'
#' @return A `mesh3d` object with the subdivided surface.
#' @export
#' @examples
#' \donttest{
#' f <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh <- read_stl(f)
#' sub <- mesh_subdivision(mesh, type = "loop", n_iterations = 1L)
#' print(sub)
#' }
mesh_subdivision <- function(mesh,
                            type              = c("sqrt3", "loop",
                                                  "catmull_clark", "doo_sabin"),
                            n_iterations      = 1L,
                            update_attributes = TRUE) {
  .validate_mesh3d(mesh)
  type_code <- switch(match.arg(type),
    catmull_clark = 0L,
    loop          = 1L,
    doo_sabin     = 2L,
    sqrt3         = 3L
  )
  rcpp_subdivide(
    mesh              = mesh,
    subdivision_type  = type_code,
    n_iterations      = as.integer(n_iterations),
    update_attributes = as.logical(update_attributes)
  )
}
