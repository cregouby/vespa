
test_that("mesh_smoothing (tangential) returns a mesh3d with same topology", {
  tet <- make_tet_mesh3d()
  result <- mesh_smoothing(tet, method = "tangential", n_iterations = 3L)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
  expect_equal(ncol(result$it), ncol(tet$it))
})


test_that("mesh_smoothing rejects non-mesh3d input", {
  expect_error(mesh_smoothing(list()), class = "rlang_error")
})
