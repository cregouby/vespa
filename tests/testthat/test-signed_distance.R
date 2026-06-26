test_that("signed_distance_function returns expected list structure", {
  tet <- make_tet_mesh3d()
  result <- signed_distance_function(tet, base_resolution = 16L)
  expect_type(result, "list")
  expect_named(result, c("dims", "spacing", "origin", "array"),
               ignore.order = TRUE)
  expect_length(result$dims, 3)
  expect_length(result$spacing, 3)
  expect_length(result$origin, 3)
})

test_that("signed_distance_function $array is a 3-D array matching $dims", {
  tet <- make_tet_mesh3d()
  result <- signed_distance_function(tet, base_resolution = 16L)
  expect_true(is.array(result$array))
  expect_equal(dim(result$array), result$dims)
  expect_equal(length(result$array), prod(result$dims))
})

test_that("signed_distance_function respects base_resolution (cubic mesh)", {
  tet <- make_tet_mesh3d()
  result <- signed_distance_function(tet, base_resolution = 10L)
  expect_true(all(result$dims >= 10L))
})

test_that("signed_distance_function padding increases grid dimensions", {
  tet <- make_tet_mesh3d()
  r0  <- signed_distance_function(tet, base_resolution = 10L, padding = 0L)
  r2  <- signed_distance_function(tet, base_resolution = 10L, padding = 2L)
  expect_true(all(r2$dims == r0$dims + 4L))
})

test_that("signed_distance_function interior point has negative SDF value", {
  tet    <- make_tet_mesh3d()
  result <- signed_distance_function(tet, base_resolution = 32L)
  center <- result$origin + result$spacing * (result$dims - 1) / 2
  ix <- round((center[1] - result$origin[1]) / result$spacing[1]) + 1L
  iy <- round((center[2] - result$origin[2]) / result$spacing[2]) + 1L
  iz <- round((center[3] - result$origin[3]) / result$spacing[3]) + 1L
  expect_lt(result$array[ix, iy, iz], 0)
})
