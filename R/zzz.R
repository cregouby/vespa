.validate_mesh3d <- function(mesh, arg = "mesh") {
  if (!inherits(mesh, "mesh3d"))
    cli::cli_abort("{.arg {arg}} must be a {.cls mesh3d} object")
  if (is.null(mesh$vb) || is.null(mesh$it))
    cli::cli_abort("{.arg {arg}} must have {.field $vb} and {.field $it} components")
  if (!is.matrix(mesh$vb) || nrow(mesh$vb) != 4L)
    cli::cli_abort("{.arg {arg}}$vb must be a 4-row matrix")
  if (!is.matrix(mesh$it) || nrow(mesh$it) != 3L)
    cli::cli_abort("{.arg {arg}}$it must be a 3-row matrix")
  invisible(NULL)
}
