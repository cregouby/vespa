/**
 * @class   vtkCGALHelper
 * @brief   helper function for VTK/CGAL compatibility
 *
 * This helper namespace defines useful functions to translate from VTK datasets
 * to CGAL surfaces back and forth.
 * This also ensures face orientation consistency during the VTK to CGAL
 * export.
 */

#ifndef vtkCGALHelper_h
#define vtkCGALHelper_h

// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>

using CGAL_Kernel  = CGAL::Exact_predicates_inexact_constructions_kernel;
using CGAL_Surface = CGAL::Surface_mesh<CGAL_Kernel::Point_3>;
using Graph_Verts  = boost::graph_traits<CGAL_Surface>::vertex_descriptor;
using Graph_Faces  = boost::graph_traits<CGAL_Surface>::face_descriptor;
using Graph_Coord  = boost::property_map<CGAL_Surface, CGAL::vertex_point_t>::type;

class ctkImageData;
class vtkPolyData;

namespace vtkCGALHelper
{
/**
 * Container for CGAL surfaces
 * Stores a set of points and triangles
 */
struct Vespa_soup
{
  std::vector<CGAL_Kernel::Point_3>     points;
  std::vector<std::vector<std::size_t>> faces;
};

/**
 * Container for CGAL surfaces
 * Stores a 2-manifold triangulation
 */
struct Vespa_surface
{
  CGAL_Surface surface;
  Graph_Coord  coords;

  Vespa_surface() { coords = get(CGAL::vertex_point, surface); }
};

/**
 * Convert a vtkPolyData to a CGAL surface mesh.
 * This method fills the internal points and cells
 * in the Vespa_soup data object.
 * return true if operation was successful.
 */
bool toCGAL(vtkPolyData* vtkMesh, Vespa_soup* cgalMesh);

/**
 * Convert a vtkPolyData to a CGAL surface mesh.
 * This method fills the internal surface and coords
 * in the Vespa_surface data object.
 * return true if operation was successful.
 */
bool toCGAL(vtkPolyData* vtkMesh, Vespa_surface* cgalMesh);

/**
 * Convert a CGAL polygon soup to a vtkPolydata.
 * return true if operation was successful.
 * result may not be manifold.
 */
bool toVTK(Vespa_soup const* cgalMesh, vtkPolyData* vtkMesh);

/**
 * Convert a CGAL surface mesh to a vtkPolydata.
 * return true if operation was successful.
 */
bool toVTK(Vespa_surface const* cgalMesh, vtkPolyData* vtkMesh);
};

#endif
