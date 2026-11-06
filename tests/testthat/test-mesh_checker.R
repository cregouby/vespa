tet <- make_tet_mesh3d()
# we remove last facet of the tet to make it invalid
invalid <- structure(list(vb = tet$vb, it = tet$it[, 1:3]), class = c("mesh3d", "shape3d"))

test_that("mesh_check returns a mesh3d for a valid mesh", {
  result <- mesh_check(tet, check_watertight = TRUE, check_intersect = TRUE)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})

test_that("mesh_check returns a mesh3d for a valid mesh when attempt repair", {
  result <- mesh_check(tet, check_watertight = TRUE, check_intersect = TRUE, attempt_repair = TRUE)
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})

test_that("mesh_check provides understandable insight for a mesh3d for a invalid mesh", {
  expect_warning(
    result <- mesh_check(invalid, check_watertight = TRUE, check_intersect = TRUE),
    "not closed"
  )
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})

test_that("mesh_check returns a mesh3d for a invalid mesh when attempt repair", {
  expect_warning(
    expect_message(
      result <- mesh_check(invalid, check_watertight = TRUE, check_intersect = TRUE, attempt_repair = TRUE),
      "successful"
    ),
    "not closed"
  )
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})
