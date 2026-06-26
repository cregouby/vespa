
test_that("mesh_check returns a mesh3d for a valid mesh", {
  tet <- make_tet_mesh3d()
  result <- mesh_check(tet, check_watertight = TRUE,
                       check_intersect = TRUE)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})
