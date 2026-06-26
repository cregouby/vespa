test_that("alpha_wrapping returns a mesh3d", {
  tet <- make_tet_mesh3d()
  result <- alpha_wrapping(tet, alpha = 1.0, offset = 0.5,
                             absolute_thresholds = TRUE)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
  expect_gt(ncol(result$it), 0)
})

test_that("mesh_subdivision (loop) increases vertex and face count", {
  tet <- make_tet_mesh3d()
  result <- mesh_subdivision(tet, type = "loop", n_iterations = 2L)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), ncol(tet$vb))
  expect_gt(ncol(result$it), ncol(tet$it))
})

test_that("mesh_subdivision (sqrt3) increases vertex and face count", {
  tet <- make_tet_mesh3d()
  result <- mesh_subdivision(tet, type = "sqrt3", n_iterations = 1L)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), ncol(tet$vb))
})

test_that("shape_smoothing returns a mesh3d with same topology", {
  tet <- make_tet_mesh3d()
  result <- shape_smoothing(tet, n_iterations = 2L)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
  expect_equal(ncol(result$it), ncol(tet$it))
})
