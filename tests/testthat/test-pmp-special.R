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

test_that("patch_filling returns a mesh3d (fill all holes)", {
  tet <- make_tet_mesh3d()
  result <- patch_filling(tet)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
})

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
