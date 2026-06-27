# Read a surface mesh or point cloud from a VTP file

Reads a `vtkPolyData` object from a VTK XML PolyData (`.vtp`) file using
VTK's XML reader. Both ASCII and binary (including binary-appended)
formats are supported.

## Usage

``` r
read_vtp(filename)
```

## Arguments

- filename:

  Path to the `.vtp` file. The file must exist and be readable.

## Value

A `mesh3d` object. `$vb` is a 4×N matrix of homogeneous vertex
coordinates; `$it` is a 3×M matrix of 1-based triangle indices. For
point-cloud VTP files (no polygon cells), `$it` is a 3×0 empty matrix.

## Examples

``` r
# \donttest{
url <- paste0(
  "https://gitlab.kitware.com/vtk/meshing/vespa/-/raw/master/",
  "Data/Testing/hand.vtp?ref_type=heads"
)
tmp <- tempfile(fileext = ".vtp")
download.file(url, tmp, quiet = TRUE)
mesh <- read_vtp(tmp)
#> Error: XMLPolyDataReader: ERROR: In ./IO/XML/vtkXMLReader.cxx, line 521
#> vtkXMLPolyDataReader (0x55aa576e4bd0): Error parsing input file.  ReadXMLInformation aborting.
#> 
print(mesh)
#> Error: object 'mesh' not found
# }
```
