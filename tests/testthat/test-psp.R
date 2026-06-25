test_that("vespa_pca_normals adds $normals to a point cloud", {
  pts    <- make_sphere_pointcloud()
  result <- vespa_pca_normals(pts)
  expect_s3_class(result, "mesh3d")
  expect_false(is.null(result$normals))
  expect_equal(nrow(result$normals), 3L)
  expect_equal(ncol(result$normals), ncol(result$vb))
})

test_that("vespa_pca_normals preserves point count (or removes unoriented)", {
  pts    <- make_sphere_pointcloud()
  result <- vespa_pca_normals(pts, delete_unoriented = FALSE)
  expect_equal(ncol(result$vb), ncol(pts$vb))
})

test_that("vespa_read_points errors on missing file", {
  expect_error(vespa_read_points("/no/such/file.xyz"), class = "rlang_error")
})

test_that("vespa_read_points reads a .xyz point cloud", {
  path <- system.file("testdata", "test_points.xyz", package = "rvespa")
  skip_if(nchar(path) == 0, "test_points.xyz not installed")
  result <- vespa_read_points(path)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
})
