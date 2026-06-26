test_that("advancing_front_reconstruction reconstructs a surface from a point cloud", {
  pts <- make_sphere_pointcloud()
  result <- advancing_front_reconstruction(pts)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
  expect_gt(ncol(result$it), 0)
})
