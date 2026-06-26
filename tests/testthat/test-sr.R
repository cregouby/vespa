test_that("advancing_front_reconstruction reconstructs a surface from a point cloud", {
  pts <- make_sphere_pointcloud()
  result <- advancing_front_reconstruction(pts)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
  expect_gt(ncol(result$it), 0)
})

test_that("poisson_reconstruction requires $normals field", {
  pts <- make_sphere_pointcloud()
  expect_error(poisson_reconstruction(pts), class = "rlang_error")
})

test_that("poisson_reconstruction reconstructs a surface after pca_normals", {
  pts    <- make_sphere_pointcloud()
  with_n <- pca_estimate_normals(pts)
  result <- poisson_reconstruction(with_n)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
  expect_gt(ncol(result$it), 0)
})
