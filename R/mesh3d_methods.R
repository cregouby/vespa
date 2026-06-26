#' @method print mesh3d
#' @export
print.mesh3d <- function(x, ...) {
  nv <- if (!is.null(x$vb)) ncol(x$vb) else 0L
  nf <- if (!is.null(x$it)) ncol(x$it) else 0L
  cat("<mesh3d>  ", nv, " vertices  ", nf, " faces\n", sep = "")
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
      n_vertices   = nv,
      n_faces      = nf,
      xrange       = bb[, 1L],
      yrange       = bb[, 2L],
      zrange       = bb[, 3L],
      diagonal     = diag_len,
      has_normals  = !is.null(object$normals)
    ),
    class = "summary.mesh3d"
  )
}

#' @method print summary.mesh3d
#' @export
print.summary.mesh3d <- function(x, ...) {
  cat("mesh3d summary\n")
  cat("  Vertices :", x$n_vertices, "\n")
  cat("  Faces    :", x$n_faces, "\n")
  cat("  X range  :", x$xrange[1L], "-", x$xrange[2L], "\n")
  cat("  Y range  :", x$yrange[1L], "-", x$yrange[2L], "\n")
  cat("  Z range  :", x$zrange[1L], "-", x$zrange[2L], "\n")
  cat("  Diagonal :", round(x$diagonal, 4L), "\n")
  cat("  Normals  :", x$has_normals, "\n")
  invisible(x)
}
