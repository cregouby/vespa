#' 2D Delaunay triangulation of a planar point set
#'
#' Computes the Delaunay triangulation of a set of 2D points using CGAL's
#' constrained Delaunay algorithm. Optional edge constraints force specific
#' edges into the triangulation.
#'
#' @param points A numeric matrix with 2 columns (x, y), one row per point.
#' @param constraints An optional integer matrix with 2 columns of 1-based
#'   vertex indices, one row per constraint edge. Default `NULL` (unconstrained
#'   Delaunay triangulation).
#'
#' @return An `sfc_POLYGON` object (from the \pkg{sf} package) containing one
#'   triangle polygon per Delaunay triangle.
#' @export
vespa_delaunay2 <- function(points, constraints = NULL) {
  points <- as.matrix(points)
  if (!is.numeric(points) || ncol(points) != 2L)
    cli::cli_abort("{.arg points} must be a numeric matrix with 2 columns (x, y)")

  constraint_edges <- if (is.null(constraints)) {
    NULL
  } else {
    m <- as.matrix(constraints)
    if (!is.integer(m)) storage.mode(m) <- "integer"
    if (ncol(m) != 2L)
      cli::cli_abort("{.arg constraints} must be a 2-column matrix of vertex indices")
    m
  }

  rings <- rcpp_delaunay2(
    points           = points,
    constraint_edges = constraint_edges
  )

  sf::st_sfc(lapply(rings, function(r) sf::st_polygon(list(r))))
}
