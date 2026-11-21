args <- commandArgs(trailingOnly = TRUE)
flag <- if (length(args)) args[1L] else "--cppflags"

vtk_modules <- c(
  "vtkIOLegacy",
  "vtkIOXML",
  "vtkIOXMLParser",
  "vtkIOCore",
  "vtkIOGeometry",
  "vtkFiltersCore",
  "vtkFiltersGeneral",
  "vtkFiltersGeometry",
  "vtkFiltersModeling",
  "vtkCommonCore",
  "vtkCommonDataModel",
  "vtkCommonExecutionModel",
  "vtkCommonMath",
  "vtkCommonMisc",
  "vtkCommonSystem",
  "vtkCommonTransforms",
  "vtksys"
)

if (flag == "--cppflags") {
  rvtk::CppFlags()
} else if (flag == "--libs") {
  rvtk::LdFlagsFile(path = "vtk_libs.rsp", modules = vtk_modules)
}
