# Signed Distance Function

``` r

library(vespa)
library(rgl)
library(tictoc)

dragon_url <- "https://gitlab.kitware.com/vtk/meshing/vespa/-/raw/master/Data/Testing/dragon.vtp"
dragon_tmp <- tempfile(fileext = ".vtp")
download.file(dragon_url, dragon_tmp, quiet = TRUE)
```

Speaking of implicit surface, the Signed Distance Function transforms a
close-meshed shape into an volume with each voxel being the distance to
the mesh surface.

The distance is positive outside of the mesh, and negative inside the
mesh.

This creates an implicit surface, defined by all the voxels being 0. In
VTK terms, you can transform a PolyData into an ImageData.

Here, many operations or visualizations are simpler like convoluting
your shape with a kernel, having a volumetric render of your object,
resampling, etc. You can then reconstruct a mesh using the Contour
filter. Choosing an isosurface value different from 0 allows you to grow
or shrink the original mesh.

The memory footprint is $`O(n^3)`$ with n being the `base_resolution`.
The `base_resolution` value is used for the smallest dimension of the
mesh, the other dimensions resolution are derived as each the voxel is
cubical.

``` r

# a) Original mesh
dragon <- read_vtp(dragon_tmp)

# Build the SDF volume — result is now an `sdf_volume` object
tic()
dragon_sdf <- signed_distance_function(dragon, base_resolution = 64L, padding = 3L)
toc()
#> 10.43 sec elapsed
dragon_sdf
#> <sdf_volume>
#>   dims:    147 x 70 x 100  (1,029,000 voxels)
#>   spacing: 0.06429 x 0.06414 x 0.06437
#>   origin:  (-4.5, -2.02, -2.993)
#>   values:  [-0.6105, 4.246]
```

After the heavy computation of the SDF for 1M points, every
transformation is trivial

``` r

# Isovalues expressed in physical units (voxel size)
iso_thicken <-  1 * dragon_sdf$spacing[1]   # outside -> grows
iso_shrink  <- -1 * dragon_sdf$spacing[1]   # inside  -> shrinks

# b) Thickened mesh
dragon_thick <- extract_isosurface(dragon_sdf, isovalue = iso_thicken)

# c) Shrunk mesh
dragon_shrunk <- extract_isosurface(dragon_sdf, isovalue = iso_shrink)
```

Now it is time to see the result :

``` r

mfrow3d(1, 3)
shade3d(dragon, color = "lightblue")
title3d(main = "a) Original mesh")
rgl::next3d()
shade3d(dragon_thick, color = "lightgreen")
title3d(main = sprintf("b) Thickened (iso = %.2f)", iso_thicken))
rgl::next3d()
shade3d(dragon_shrunk, color = "salmon")
title3d(main = sprintf("c) Shrunk (iso = %.2f)", iso_shrink))
```

![](images/clipboard-325478204.png)
