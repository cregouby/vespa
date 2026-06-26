test_that("mesh3d → vtkPolyData → mesh3d round-trip is identity", {
  tet <- make_tet_mesh3d()
  result <- vespa:::rcpp_roundtrip(tet)

  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))  # vertex count preserved
  expect_equal(ncol(result$it), ncol(tet$it))  # face count preserved
  expect_equal(result$vb, tet$vb, tolerance = 1e-6)  # VTK stores float32
  expect_equal(result$it, tet$it)              # face indices exact
})

test_that("round-trip preserves w=1 in homogeneous column", {
  tet <- make_tet_mesh3d()
  result <- vespa:::rcpp_roundtrip(tet)
  expect_true(all(result$vb[4, ] == 1))
})
