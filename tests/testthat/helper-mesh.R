# Reference tetrahedron fixture — no external dependencies required.
# $vb : 4×4 matrix, each column = (x, y, z, w=1)
# $it : 3×4 matrix, each column = face (1-based vertex indices, outward CCW)
make_tet_mesh3d <- function() {
  s <- 1 / sqrt(2)
  vb <- matrix(
    c(
      1,
      0,
      -s,
      1,
      -1,
      0,
      -s,
      1,
      0,
      1,
      s,
      1,
      0,
      -1,
      s,
      1
    ),
    nrow = 4
  )
  # Faces oriented with outward-pointing normals (CCW when seen from outside)
  it <- matrix(
    c(
      1,
      2,
      3,
      1,
      4,
      2,
      1,
      3,
      4,
      2,
      4,
      3
    ),
    nrow = 3
  )
  structure(list(vb = vb, it = it), class = c("mesh3d", "shape3d"))
}

# Non-watertight mesh — tetrahedron with one face removed (open boundary).
make_open_mesh3d <- function(mesh, hole = 1) {
  n_open_it <- ncol(mesh$it) - hole
  it_open_ids <- sample(ncol(mesh$it), n_open_it)
  structure(list(vb = mesh$vb, it = mesh$it[, it_open_ids]), class = c("mesh3d", "shape3d"))
}

# Self-intersecting mesh — two overlapping closed tetrahedra merged into one mesh.
# Each tetrahedron is individually watertight; faces from one cross faces of the other.
make_self_intersecting_mesh3d <- function() {
  s <- 1 / sqrt(2)
  vb1 <- matrix(
    c(
      1,
      0,
      -s,
      1,
      -1,
      0,
      -s,
      1,
      0,
      1,
      s,
      1,
      0,
      -1,
      s,
      1
    ),
    nrow = 4
  )
  vb2 <- vb1
  vb2[1, ] <- vb2[1, ] + 0.6 # shift x so the two tets partially overlap
  it1 <- matrix(c(1, 3, 2, 1, 2, 4, 1, 4, 3, 2, 3, 4), nrow = 3)
  it2 <- it1 + 4L
  structure(
    list(vb = cbind(vb1, vb2), it = cbind(it1, it2)),
    class = c("mesh3d", "shape3d")
  )
}

# Non-manifold mesh — two tetrahedra sharing a single apex vertex (bowtie).
# Non-manifold at the shared vertex; also non-watertight.
make_bowtie_mesh3d <- function() {
  vb <- matrix(
    c(
      0,
      0,
      0,
      1, # shared apex (vertex 1)
      1,
      0,
      0,
      1,
      0,
      1,
      0,
      1,
      0,
      0,
      1,
      1,
      -1,
      0,
      0,
      1,
      0,
      -1,
      0,
      1,
      0,
      0,
      -1,
      1
    ),
    nrow = 4
  )
  it1 <- matrix(c(1, 2, 3, 1, 3, 4, 1, 4, 2, 2, 4, 3), nrow = 3)
  it2 <- matrix(c(1, 5, 6, 1, 6, 7, 1, 7, 5, 5, 7, 6), nrow = 3)
  structure(
    list(vb = vb, it = cbind(it1, it2)),
    class = c("mesh3d", "shape3d")
  )
}

# Icosphere point cloud — 42 points sampled on the unit sphere.
# Useful as input for surface reconstruction tests (no connectivity: $it is 3×0).
make_sphere_pointcloud <- function() {
  phi <- (1 + sqrt(5)) / 2
  verts <- rbind(
    c(0, 1, phi),
    c(0, -1, phi),
    c(0, 1, -phi),
    c(0, -1, -phi),
    c(1, phi, 0),
    c(-1, phi, 0),
    c(1, -phi, 0),
    c(-1, -phi, 0),
    c(phi, 0, 1),
    c(-phi, 0, 1),
    c(phi, 0, -1),
    c(-phi, 0, -1)
  )
  verts <- verts / sqrt(rowSums(verts^2))
  vb <- rbind(t(verts), 1)
  structure(
    list(vb = vb, it = matrix(integer(0), 3L, 0L)),
    class = c("mesh3d", "shape3d")
  )
}
