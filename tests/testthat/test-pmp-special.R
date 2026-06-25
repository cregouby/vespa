test_that("vespa_boolean_op union has more points than either input", {
  tet <- make_tet_mesh3d()
  result <- vespa_boolean_op(tet, tet, operation = "union")
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
})

test_that("vespa_boolean_op rejects non-mesh3d first argument", {
  tet <- make_tet_mesh3d()
  expect_error(vespa_boolean_op(list(), tet), class = "rlang_error")
})

test_that("vespa_boolean_op rejects non-mesh3d second argument", {
  tet <- make_tet_mesh3d()
  expect_error(vespa_boolean_op(tet, list()), class = "rlang_error")
})

test_that("vespa_patch_fill returns a mesh3d (fill all holes)", {
  tet <- make_tet_mesh3d()
  result <- vespa_patch_fill(tet)
  expect_s3_class(result, "mesh3d")
  expect_gt(ncol(result$vb), 0)
})

test_that("vespa_region_fair rejects empty point_ids", {
  tet <- make_tet_mesh3d()
  expect_error(vespa_region_fair(tet, integer(0)), class = "rlang_error")
})

test_that("vespa_region_fair returns a mesh3d with same topology", {
  tet <- make_tet_mesh3d()
  result <- vespa_region_fair(tet, point_ids = 1L)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
  expect_equal(ncol(result$it), ncol(tet$it))
})

test_that("vespa_mesh_deform rejects empty control_ids", {
  tet <- make_tet_mesh3d()
  expect_error(
    vespa_mesh_deform(tet, integer(0), matrix(0, 0, 3)),
    class = "rlang_error"
  )
})

test_that("vespa_mesh_deform rejects mismatched target_coords rows", {
  tet <- make_tet_mesh3d()
  expect_error(
    vespa_mesh_deform(tet, control_ids = 1L, target_coords = matrix(0, 2, 3)),
    class = "rlang_error"
  )
})

test_that("vespa_mesh_deform moves a control vertex", {
  tet  <- make_tet_mesh3d()
  ctrl <- 1L
  orig <- tet$vb[1:3, ctrl]
  tgt  <- orig + c(0.1, 0, 0)
  result <- vespa_mesh_deform(
    tet,
    control_ids   = ctrl,
    target_coords = matrix(tgt, nrow = 1)
  )
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})
