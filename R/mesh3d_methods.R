#' @method print mesh3d
#' @export
print.mesh3d <- function(x, ...) {
  nv <- if (!is.null(x$vb)) ncol(x$vb) else 0L
  nf <- if (!is.null(x$it)) ncol(x$it) else 0L
  cli::cli_text(
    "{.cls mesh3d}  \
     {.val {nv}} {cli::qty(nv)}{?vertex/vertices}  \
     {.val {nf}} {cli::qty(nf)}{?face/faces}"
  )
  invisible(x)
}

#' @method summary mesh3d
#' @export
summary.mesh3d <- function(object, ...) {
  nv <- if (!is.null(object$vb)) ncol(object$vb) else 0L
  nf <- if (!is.null(object$it)) ncol(object$it) else 0L
  coords <- if (nv > 0L) object$vb[1:3, , drop = FALSE] else matrix(NA_real_, 3L, 0L)
  bb <- if (nv > 0L) apply(coords, 1L, range) else matrix(NA_real_, 2L, 3L)
  diag_len <- if (nv > 0L) sqrt(sum((bb[2L, ] - bb[1L, ])^2)) else NA_real_
  structure(
    list(
      n_vertices  = nv,
      n_faces     = nf,
      xrange      = bb[, 1L],
      yrange      = bb[, 2L],
      zrange      = bb[, 3L],
      diagonal    = diag_len,
      has_normals = !is.null(object$normals)
    ),
    class = "summary.mesh3d"
  )
}

#' @method print summary.mesh3d
#' @export
print.summary.mesh3d <- function(x, ...) {
  fmt_range <- function(r) sprintf("[%g, %g]", signif(r[1L], 5L), signif(r[2L], 5L))
  normals_entry <- if (x$has_normals) "{.field yes}" else cli::style_dim("no")
  cli::cli_h3("mesh3d")
  cli::cli_dl(c(
    "Vertices" = "{.val {x$n_vertices}}",
    "Faces"    = "{.val {x$n_faces}}"
  ))
  cli::cli_dl(c(
    "X range"  = fmt_range(x$xrange),
    "Y range"  = fmt_range(x$yrange),
    "Z range"  = fmt_range(x$zrange),
    "Diagonal" = "{.val {signif(x$diagonal, 5L)}}"
  ))
  cli::cli_dl(c("Normals" = normals_entry))
  invisible(x)
}
