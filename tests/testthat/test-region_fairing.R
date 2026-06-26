test_that("region_fairing rejects empty point_ids", {
  tet <- make_tet_mesh3d()
  expect_error(region_fairing(tet, integer(0)), class = "rlang_error")
})

test_that("region_fairing returns a mesh3d with same topology", {
  tet <- make_tet_mesh3d()
  result <- region_fairing(tet, point_ids = 1L)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
  expect_equal(ncol(result$it), ncol(tet$it))
})
