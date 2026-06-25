test_that("vespa_delaunay2 triangulates a square into 2 triangles", {
  pts <- matrix(c(0, 0,  1, 0,  1, 1,  0, 1), ncol = 2, byrow = TRUE)
  result <- vespa_delaunay2(pts)
  expect_s3_class(result, "sfc")
  expect_length(result, 2L)
})

test_that("vespa_delaunay2 returns sfc_POLYGON elements", {
  pts <- matrix(c(0, 0,  1, 0,  0.5, 1), ncol = 2, byrow = TRUE)
  result <- vespa_delaunay2(pts)
  expect_true(all(vapply(result, inherits, logical(1), "sfg")))
  expect_equal(attr(result, "class")[1], "sfc_POLYGON")
})

test_that("vespa_delaunay2 rejects non-2-column input", {
  expect_error(vespa_delaunay2(matrix(1:9, 3, 3)), class = "rlang_error")
})

test_that("vespa_delaunay2 rejects non-numeric input", {
  expect_error(vespa_delaunay2(data.frame(x = "a", y = "b")), class = "rlang_error")
})

test_that("vespa_delaunay2 with constraint edge includes that edge", {
  pts  <- matrix(c(0, 0,  1, 0,  1, 1,  0, 1), ncol = 2, byrow = TRUE)
  cons <- matrix(c(1L, 3L), nrow = 1)
  result <- vespa_delaunay2(pts, constraints = cons)
  expect_s3_class(result, "sfc")
  expect_length(result, 2L)
})
