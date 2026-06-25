test_that("vespa_advancing_front reconstructs a surface from a point cloud", {
  pts <- make_sphere_pointcloud()
  result <- vespa_advancing_front(pts)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
  expect_gt(ncol(result$it), 0)
})

test_that("vespa_poisson_recon requires $normals field", {
  pts <- make_sphere_pointcloud()
  expect_error(vespa_poisson_recon(pts), class = "rlang_error")
})

test_that("vespa_poisson_recon reconstructs a surface after pca_normals", {
  pts    <- make_sphere_pointcloud()
  with_n <- vespa_pca_normals(pts)
  result <- vespa_poisson_recon(with_n)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
  expect_gt(ncol(result$it), 0)
})
