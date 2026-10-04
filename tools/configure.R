args <- commandArgs(trailingOnly = TRUE)
flag <- if (length(args)) args[1L] else "--cppflags"

vtk_modules <- c(
  "vtkIOLegacy",
  "vtkIOXML",
  "vtkIOXMLParser",
  "vtkIOCore",
  "vtkIOGeometry",
  "vtkCommonCore",
  "vtkCommonDataModel",
  "vtkCommonExecutionModel",
  "vtkCommonMath",
  "vtkCommonMisc",
  "vtkCommonSystem",
  "vtkCommonTransforms",
  "vtkFiltersCore",
  "vtkFiltersGeneral",
  "vtkFiltersGeometry",
  "vtkFiltersExtraction",
  "vtkFiltersModeling",
  "vtkFiltersPoints",
  "vtksys"
)

if (flag == "--cppflags") {
  rvtk::CppFlags()
} else if (flag == "--libs") {
  rvtk::LdFlagsFile(path = "vtk_libs.rsp", modules = vtk_modules)
}

