test_that("extract_isosurface returns a valid mesh3d", {
  skip_if_not_installed("rgl")
  
  # Create a triangulated test mesh (icosahedron has only triangles)
  mesh <- rgl::icosahedron3d()
  
  # Compute SDF
  sdf <- signed_distance_function(mesh, base_resolution = 32L, padding = 2L)
  
  # Extract isosurface at level 0
  result <- extract_isosurface(sdf, isovalue = 0)
  
  # Check it's a mesh3d
  expect_s3_class(result, "mesh3d")
  
  # Check $it has 3 rows (triangle indices)
  expect_equal(nrow(result$it), 3)
  
  # Check $vb has 4 rows (homogeneous coordinates)
  expect_equal(nrow(result$vb), 4)
  
  # Check we have triangles
  expect_gt(ncol(result$it), 0)
  expect_gt(ncol(result$vb), 0)
})

test_that("extract_isosurface with positive isovalue grows the mesh", {
  skip_if_not_installed("rgl")
  
  mesh <- rgl::icosahedron3d()
  sdf <- signed_distance_function(mesh, base_resolution = 32L, padding = 5L)
  
  # Extract at isovalue 0 (original size)
  mesh_0 <- extract_isosurface(sdf, isovalue = 0)
  
  # Extract at positive isovalue (should be larger)
  iso_pos <- 2 * sdf$spacing[1]
  mesh_pos <- extract_isosurface(sdf, isovalue = iso_pos)
  
  # Compute bounding box sizes manually
  bbox_0 <- apply(mesh_0$vb[1:3, ], 1, range)
  size_0 <- bbox_0[2, ] - bbox_0[1, ]
  
  bbox_pos <- apply(mesh_pos$vb[1:3, ], 1, range)
  size_pos <- bbox_pos[2, ] - bbox_pos[1, ]
  
  # The positive isovalue mesh should be larger
  expect_true(all(size_pos > size_0))
})

test_that("extract_isosurface with negative isovalue shrinks the mesh", {
  skip_if_not_installed("rgl")
  
  mesh <- rgl::icosahedron3d()
  sdf <- signed_distance_function(mesh, base_resolution = 32L, padding = 5L)
  
  # Extract at isovalue 0
  mesh_0 <- extract_isosurface(sdf, isovalue = 0)
  
  # Extract at negative isovalue (should be smaller)
  iso_neg <- -2 * sdf$spacing[1]
  mesh_neg <- extract_isosurface(sdf, isovalue = iso_neg)
  
  # Compute bounding box sizes manually
  bbox_0 <- apply(mesh_0$vb[1:3, ], 1, range)
  size_0 <- bbox_0[2, ] - bbox_0[1, ]
  
  bbox_neg <- apply(mesh_neg$vb[1:3, ], 1, range)
  size_neg <- bbox_neg[2, ] - bbox_neg[1, ]
  
  # The negative isovalue mesh should be smaller
  expect_true(all(size_neg < size_0))
})

test_that("extract_isosurface rejects non-sdf_volume input", {
  skip_if_not_installed("rgl")
  
  mesh <- rgl::icosahedron3d()
  
  # Should fail with helpful message for mesh3d
  expect_error(
    extract_isosurface(mesh, isovalue = 0),
    "expects a volumetric field"
  )
  
  # Should also fail for other types
  expect_error(
    extract_isosurface(list(a = 1, b = 2), isovalue = 0),
    "no method for objects of class"
  )
})

test_that("extract_isosurface preserves spatial coordinates", {
  skip_if_not_installed("rgl")
  
  # Create a mesh centered at origin
  mesh <- rgl::icosahedron3d()
  
  sdf <- signed_distance_function(mesh, base_resolution = 32L, padding = 3L)
  result <- extract_isosurface(sdf, isovalue = 0)
  
  # The extracted mesh should be roughly centered at origin
  center <- colMeans(t(result$vb[1:3, ]))
  
  # Allow some tolerance due to discretization
  expect_lt(abs(center[1]), 0.5)
  expect_lt(abs(center[2]), 0.5)
  expect_lt(abs(center[3]), 0.5)
})