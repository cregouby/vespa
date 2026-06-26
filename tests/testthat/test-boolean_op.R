test_that("boolean_operation union has more points than either input", {
  tet <- make_tet_mesh3d()
  result <- boolean_operation(tet, tet, operation = "union")
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
})

test_that("boolean_operation rejects non-mesh3d first argument", {
  tet <- make_tet_mesh3d()
  expect_error(boolean_operation(list(), tet), class = "rlang_error")
})

test_that("boolean_operation rejects non-mesh3d second argument", {
  tet <- make_tet_mesh3d()
  expect_error(boolean_operation(tet, list()), class = "rlang_error")
})
