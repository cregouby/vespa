test_that("vespa_alpha_wrap returns a mesh3d", {
  tet <- make_tet_mesh3d()
  result <- vespa_alpha_wrap(tet, alpha = 1.0, offset = 0.5,
                             absolute_thresholds = TRUE)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
  expect_gt(ncol(result$it), 0)
})

test_that("vespa_mesh_smooth (tangential) returns a mesh3d with same topology", {
  tet <- make_tet_mesh3d()
  result <- vespa_mesh_smooth(tet, method = "tangential", n_iterations = 3L)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
  expect_equal(ncol(result$it), ncol(tet$it))
})

test_that("vespa_subdivide (loop) increases vertex and face count", {
  tet <- make_tet_mesh3d()
  result <- vespa_subdivide(tet, type = "loop", n_iterations = 2L)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), ncol(tet$vb))
  expect_gt(ncol(result$it), ncol(tet$it))
})

test_that("vespa_subdivide (sqrt3) increases vertex and face count", {
  tet <- make_tet_mesh3d()
  result <- vespa_subdivide(tet, type = "sqrt3", n_iterations = 1L)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), ncol(tet$vb))
})

test_that("vespa_shape_smooth returns a mesh3d with same topology", {
  tet <- make_tet_mesh3d()
  result <- vespa_shape_smooth(tet, n_iterations = 2L)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
  expect_equal(ncol(result$it), ncol(tet$it))
})

test_that("vespa_mesh_check returns a mesh3d for a valid mesh", {
  tet <- make_tet_mesh3d()
  result <- vespa_mesh_check(tet, check_watertight = TRUE,
                             check_intersect = TRUE)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})

test_that("vespa_mesh_smooth rejects non-mesh3d input", {
  expect_error(vespa_mesh_smooth(list()), class = "rlang_error")
})
