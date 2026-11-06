tet <- make_tet_mesh3d()
open_tet <- make_open_mesh3d(tet)
self_int <- make_self_intersecting_mesh3d()
open_self_int <- make_open_mesh3d(self_int)
bowtie <- make_bowtie_mesh3d()
open_bowtie <- make_open_mesh3d(bowtie)

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

test_that("mesh_check warns for a non-watertight mesh", {
  expect_warning(
    result <- mesh_check(open_tet, check_watertight = TRUE, check_intersect = TRUE),
    "open boundary"
  )
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})

test_that("mesh_check repairs a non-watertight mesh with a single inform", {
  expect_message(
    result <- mesh_check(open_tet, check_watertight = TRUE, check_intersect = TRUE, attempt_repair = TRUE),
    "open boundary edges detected and successfully closed"
  )
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})

test_that("mesh_check warns for a self-intersecting mesh", {
  expect_warning(
    result <- mesh_check(self_int, check_watertight = FALSE, check_intersect = TRUE),
    regexp = "self-intersecting faces"
  )
  expect_s3_class(result, "mesh3d")
})

test_that("mesh_check repairs a self-intersecting mesh with a single inform", {
  expect_message(
    result <- mesh_check(self_int, check_watertight = TRUE, check_intersect = TRUE, attempt_repair = TRUE),
    "open boundary edges detected and successfully closed"
  )
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})

test_that("mesh_check warns for a non-manifold (bowtie) mesh", {
  expect_warning(
    result <- mesh_check(bowtie, check_watertight = TRUE, check_intersect = FALSE),
    regexp = "manifold"
  )
  expect_s3_class(result, "mesh3d")
})

test_that("mesh_check repairs a non-manifold (bowtie) mesh with a single inform", {
  expect_message(
    result <- mesh_check(bowtie, check_watertight = TRUE, check_intersect = TRUE, attempt_repair = TRUE),
    "open boundary edges detected and successfully closed"
  )
  expect_s3_class(result, "mesh3d")
  expect_equal(ncol(result$vb), ncol(tet$vb))
})
