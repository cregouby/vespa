test_that("vespa_isotropic_remesh returns a mesh3d", {
  tet <- make_tet_mesh3d()
  result <- vespa_isotropic_remesh(tet, n_iterations = 1L)
  expect_s3_class(result, "mesh3d")
  expect_true(is.matrix(result$vb))
  expect_true(is.matrix(result$it))
})

test_that("small target_length produces more vertices than the original", {
  tet <- make_tet_mesh3d()
  # protect_angle=180 disables feature edge locking so splitting is allowed
  result <- vespa_isotropic_remesh(
    tet, target_length = 0.3, protect_angle = 180, n_iterations = 3L
  )
  expect_gt(ncol(result$vb), ncol(tet$vb))
})

test_that("vespa_isotropic_remesh rejects non-mesh3d input", {
  expect_error(vespa_isotropic_remesh(list(a = 1)), class = "rlang_error")
})
