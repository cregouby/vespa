test_that("mesh_deformation rejects empty control_ids", {
  tet <- make_tet_mesh3d()
  expect_error(
    mesh_deformation(tet, integer(0), matrix(0, 0, 3)),
    class = "rlang_error"
  )
})

test_that("mesh_deformation rejects mismatched target_coords rows", {
  tet <- make_tet_mesh3d()
  expect_error(
    mesh_deformation(tet, control_ids = 1L, target_coords = matrix(0, 2, 3)),
    class = "rlang_error"
  )
})

test_that("mesh_deformation moves a control vertex", {
  tet  <- make_tet_mesh3d()
  ctrl <- 1L
  orig <- tet$vb[1:3, ctrl]
  tgt  <- orig + c(0.1, 0, 0)
  result <- mesh_deformation(
    tet,
    control_ids   = ctrl,
    target_coords = matrix(tgt, nrow = 1)
  )
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})
