tet_vtp <- system.file("testdata", "tet.vtp", package = "vespa")

test_that("read_vtp returns a mesh3d with correct structure", {
  mesh <- read_vtp(tet_vtp)
  expect_s3_class(mesh, "mesh3d")
  expect_true(is.matrix(mesh$vb))
  expect_true(is.matrix(mesh$it))
  expect_equal(nrow(mesh$vb), 4L)
  expect_equal(nrow(mesh$it), 3L)
})

test_that("read_vtp returns correct vertex and face counts for tet.vtp", {
  mesh <- read_vtp(tet_vtp)
  expect_equal(ncol(mesh$vb), 4L)
  expect_equal(ncol(mesh$it), 4L)
})

test_that("read_vtp homogeneous coordinate is 1 for all vertices", {
  mesh <- read_vtp(tet_vtp)
  expect_true(all(mesh$vb[4L, ] == 1))
})

test_that("read_vtp triangle indices are 1-based and in range", {
  mesh <- read_vtp(tet_vtp)
  expect_true(all(mesh$it >= 1L))
  expect_true(all(mesh$it <= ncol(mesh$vb)))
})

test_that("read_vtp errors on a missing file", {
  expect_error(read_vtp("no_such_file.vtp"), class = "rlang_error")
})
