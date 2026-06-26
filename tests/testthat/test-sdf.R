test_that("vespa_sdf returns expected list structure", {
  tet <- make_tet_mesh3d()
  result <- vespa_sdf(tet, base_resolution = 16L)
  expect_type(result, "list")
  expect_named(result, c("dims", "spacing", "origin", "values", "array"),
               ignore.order = TRUE)
  expect_length(result$dims, 3)
  expect_length(result$spacing, 3)
  expect_length(result$origin, 3)
  expect_equal(length(result$values), prod(result$dims))
})

test_that("vespa_sdf $array dimensions match $dims", {
  tet <- make_tet_mesh3d()
  result <- vespa_sdf(tet, base_resolution = 16L)
  expect_equal(dim(result$array), result$dims)
})

test_that("vespa_sdf respects base_resolution (cubic mesh)", {
  tet <- make_tet_mesh3d()
  result <- vespa_sdf(tet, base_resolution = 10L)
  expect_true(all(result$dims >= 10L))
})

test_that("vespa_sdf padding increases grid dimensions", {
  tet   <- make_tet_mesh3d()
  r0    <- vespa_sdf(tet, base_resolution = 10L, padding = 0L)
  r2    <- vespa_sdf(tet, base_resolution = 10L, padding = 2L)
  expect_true(all(r2$dims == r0$dims + 4L))
})

test_that("vespa_sdf interior point has negative SDF value", {
  tet    <- make_tet_mesh3d()
  result <- vespa_sdf(tet, base_resolution = 32L)
  center <- result$origin + result$spacing * (result$dims - 1) / 2
  # find voxel index closest to the centroid
  ix <- round((center[1] - result$origin[1]) / result$spacing[1]) + 1L
  iy <- round((center[2] - result$origin[2]) / result$spacing[2]) + 1L
  iz <- round((center[3] - result$origin[3]) / result$spacing[3]) + 1L
  val <- result$array[ix, iy, iz]
  expect_lt(val, 0)
})
