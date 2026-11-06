#' Validate and optionally repair a triangular surface mesh
#'
#' Checks a mesh for watertightness and self-intersections using CGAL's
#' polygon mesh processing algorithms. Issues are reported via `cli` messages.
#' When `attempt_repair = TRUE`, Vespa attempts to fix detected problems and
#' returns the repaired mesh.
#'
#' @param mesh A `mesh3d` object.
#' @param check_watertight Whether to check if the mesh is closed and bounds a
#'   volume. Default `TRUE`.
#' @param check_intersect Whether to check for self-intersections. Default
#'   `TRUE`.
#' @param attempt_repair If `TRUE`, Vespa attempts to fix detected issues
#'   (fill holes, re-orient faces, remove self-intersections). Default `FALSE`.
#'
#' @return A `mesh3d` object — identical to the input unless `attempt_repair =
#'   TRUE` and repairs were applied.
#' @export
#' @examples
#' f <- system.file("extdata", "torus.stl", package = "vespa")
#' mesh <- read_stl(f)
#' result <- mesh_check(mesh)
#' @family mesh_transforms
mesh_check <- function(
  mesh,
  check_watertight = TRUE,
  check_intersect = TRUE,
  attempt_repair = FALSE
) {
  .validate_mesh3d(mesh)

  vtk_msgs <- character(0)
  crashed   <- FALSE
  result <- tryCatch(
    withCallingHandlers(
      rcpp_mesh_check(
        mesh = mesh,
        check_watertight = as.logical(check_watertight),
        check_intersect = as.logical(check_intersect),
        attempt_repair = as.logical(attempt_repair)
      ),
      warning = function(w) {
        vtk_msgs <<- c(vtk_msgs, conditionMessage(w))
        invokeRestart("muffleWarning")
      }
    ),
    error = function(e) { crashed <<- TRUE; NULL }
  )

  parsed <- lapply(vtk_msgs, .parse_vtk_msg)

  if (attempt_repair) {
    any_failure <- crashed || any(vapply(parsed, `[[`, logical(1), "is_failure"))
    descriptions <- vapply(parsed, `[[`, character(1), "description")
    if (crashed)
      descriptions <- c(descriptions, "repair {.strong failed}: mesh cannot be processed")
    if (length(descriptions) > 0) {
      combined <- paste(descriptions, collapse = " and ")
      if (any_failure) cli::cli_warn(combined) else cli::cli_inform(combined)
    }
  } else {
    for (p in parsed) {
      if (p$is_success) cli::cli_inform(p$description) else cli::cli_warn(p$description)
    }
  }

  if (is.null(result)) return(invisible(mesh))
  result
}

# Parse a raw VTK warning string into a short combinable description.
# VTK format (two lines): "In File.cxx, line N\nClassName (0xADDR): raw message"
.parse_vtk_msg <- function(msg) {
  lines <- trimws(strsplit(msg, "[\r\n]+")[[1]])
  lines <- lines[nzchar(lines)]

  raw <- if (length(lines) >= 2) {
    sub("^\\S+\\s+\\([^)]+\\):\\s*", "", lines[2])
  } else {
    sub("^[^:]+:\\s*", "", lines[1])
  }
  raw <- trimws(raw)

  if (grepl("not closed", raw, ignore.case = TRUE)) {
    list(description = "{.strong open boundary} edges detected",
         is_success = FALSE, is_failure = FALSE)
  } else if (grepl("self.intersect", raw, ignore.case = TRUE)) {
    list(description = "{.strong self-intersecting} faces detected",
         is_success = FALSE, is_failure = FALSE)
  } else if (grepl("polygon soup", raw, ignore.case = TRUE)) {
    list(description = "mesh is not {.strong manifold}: consider Alpha Wrapping",
         is_success = FALSE, is_failure = FALSE)
  } else if (grepl("closing successful|closed successfully", raw, ignore.case = TRUE)) {
    list(description = "successfully closed",
         is_success = TRUE, is_failure = FALSE)
  } else if (grepl("closing failed|remove intersection failed", raw, ignore.case = TRUE)) {
    list(description = "repair {.strong failed}",
         is_success = FALSE, is_failure = TRUE)
  } else {
    list(description = raw, is_success = FALSE, is_failure = FALSE)
  }
}
