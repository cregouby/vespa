# Safety net for bundled Vespa libs on Linux (rpath handles macOS)
.onLoad <- function(libname, pkgname) {
  vespa_lib <- system.file("vespa", "lib", package = pkgname)
  if (
    nzchar(vespa_lib) &&
      dir.exists(vespa_lib) &&
      Sys.info()[["sysname"]] == "Linux"
  ) {
    old <- Sys.getenv("LD_LIBRARY_PATH", unset = "")
    new <- if (nzchar(old)) paste(vespa_lib, old, sep = ":") else vespa_lib
    Sys.setenv(LD_LIBRARY_PATH = new)
  }
}

.validate_mesh3d <- function(mesh, arg = "mesh") {
  if (!inherits(mesh, "mesh3d")) {
    cli::cli_abort("{.arg {arg}} must be a {.cls mesh3d} object")
  }
  if (is.null(mesh$vb) || is.null(mesh$it)) {
    cli::cli_abort(
      "{.arg {arg}} must have {.field $vb} and {.field $it} components"
    )
  }
  if (!is.matrix(mesh$vb) || nrow(mesh$vb) != 4L) {
    cli::cli_abort("{.arg {arg}}$vb must be a 4-row matrix")
  }
  if (!is.matrix(mesh$it) || nrow(mesh$it) != 3L) {
    cli::cli_abort("{.arg {arg}}$it must be a 3-row matrix")
  }
  invisible(NULL)
}
