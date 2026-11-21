test_that("patch_filling returns a mesh3d (fill all holes)", {
  tet <- make_tet_mesh3d()
  result <- patch_filling(tet)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
})

test_that("patch_filling is a no-op on a closed mesh", {
  tet <- make_tet_mesh3d()
  result <- patch_filling(tet)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
  expect_equal(ncol(result$it), ncol(tet$it))
})

test_that("patch_filling fills an open boundary", {
  tet <- make_tet_mesh3d()
  open_tet <- structure(
    list(vb = tet$vb, it = tet$it[, c(1, 2, 3)]),
    class = c("mesh3d", "shape3d")
  )
  result <- patch_filling(open_tet)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$it), ncol(open_tet$it))
})
