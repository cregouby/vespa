test_that("patch_filling returns a mesh3d (fill all holes)", {
  tet <- make_tet_mesh3d()
  result <- patch_filling(tet)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
})
