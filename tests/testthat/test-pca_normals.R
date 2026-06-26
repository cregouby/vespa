test_that("pca_estimate_normals adds $normals to a point cloud", {
  pts    <- make_sphere_pointcloud()
  result <- pca_estimate_normals(pts)
  expect_s3_class(result, "mesh3d")
  expect_false(is.null(result$normals))
  expect_equal(nrow(result$normals), 3L)
  expect_equal(ncol(result$normals), ncol(result$vb))
})

test_that("pca_estimate_normals preserves point count (or removes unoriented)", {
  pts    <- make_sphere_pointcloud()
  result <- pca_estimate_normals(pts, delete_unoriented = FALSE)
  expect_equal(ncol(result$vb), ncol(pts$vb))
})

test_that("read_points_xyz errors on missing file", {
  expect_error(read_points_xyz("/no/such/file.xyz"), class = "rlang_error")
})

test_that("read_points_xyz reads a .xyz point cloud", {
  path <- system.file("testdata", "test_points.xyz", package = "vespa")
  skip_if(nchar(path) == 0, "test_points.xyz not installed")
  result <- read_points_xyz(path)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
})
