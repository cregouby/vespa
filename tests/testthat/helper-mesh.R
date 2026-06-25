# Reference tetrahedron fixture — no external dependencies required.
# $vb : 4×4 matrix, each column = (x, y, z, w=1)
# $it : 3×4 matrix, each column = face (1-based vertex indices, outward CCW)
make_tet_mesh3d <- function() {
  s <- 1 / sqrt(2)
  vb <- matrix(c(
     1,  0, -s, 1,
    -1,  0, -s, 1,
     0,  1,  s, 1,
     0, -1,  s, 1
  ), nrow = 4)
  # Faces oriented with outward-pointing normals (CCW when seen from outside)
  it <- matrix(c(
    1, 2, 3,
    1, 4, 2,
    1, 3, 4,
    2, 4, 3
  ), nrow = 3)
  structure(list(vb = vb, it = it), class = c("mesh3d", "shape3d"))
}

# Icosphere point cloud — 42 points sampled on the unit sphere.
# Useful as input for surface reconstruction tests (no connectivity: $it is 3×0).
make_sphere_pointcloud <- function() {
  phi <- (1 + sqrt(5)) / 2
  verts <- rbind(
    c( 0,  1,  phi), c( 0, -1,  phi), c( 0,  1, -phi), c( 0, -1, -phi),
    c( 1,  phi,  0), c(-1,  phi,  0), c( 1, -phi,  0), c(-1, -phi,  0),
    c( phi,  0,  1), c(-phi,  0,  1), c( phi,  0, -1), c(-phi,  0, -1)
  )
  verts <- verts / sqrt(rowSums(verts^2))
  vb <- rbind(t(verts), 1)
  structure(
    list(vb = vb, it = matrix(integer(0), 3L, 0L)),
    class = c("mesh3d", "shape3d")
  )
}
