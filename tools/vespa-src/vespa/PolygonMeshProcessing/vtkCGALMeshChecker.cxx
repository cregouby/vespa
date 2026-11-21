#include "vtkCGALMeshChecker.h"

// VESPA related includes
#include "vtkCGALHelper.h"

// VTK related includes
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

// CGAL related includes
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
// hole fairing
#include <CGAL/version.h>
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(6,2,0)
#  include <CGAL/boost/graph/border.h>
#else
#  include <CGAL/Polygon_mesh_processing/border.h>
#endif
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include <exception>
#include <iterator>
#include <memory>
#include <vector>

vtkStandardNewMacro(vtkCGALMeshChecker);

namespace pmp = CGAL::Polygon_mesh_processing;

//------------------------------------------------------------------------------
void vtkCGALMeshChecker::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "CheckWatertight: " << (this->CheckWatertight ? "True" : "False") << std::endl;
  os << indent << "CheckIntersect: " << (this->CheckIntersect ? "True" : "False") << std::endl;
  os << indent << "AttemptRepair: " << (this->AttemptRepair ? "True" : "False") << std::endl;
}

//------------------------------------------------------------------------------
int vtkCGALMeshChecker::RequestData(
  vtkInformation*, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  using Graph_halfedge = boost::graph_traits<CGAL_Surface>::halfedge_descriptor;

  // Get the input and source data object
  vtkPolyData* input = vtkPolyData::GetData(inputVector[0]);

  if (!input)
  {
    vtkErrorMacro("Missing input mesh.");
  }

  vtkPolyData* output = vtkPolyData::GetData(outputVector);

  // Create the soup meshes for CGAL
  // ----------------------------------

  std::unique_ptr<vtkCGALHelper::Vespa_soup> cgalSoup =
    std::make_unique<vtkCGALHelper::Vespa_soup>();
  vtkCGALHelper::toCGAL(input, cgalSoup.get());
  std::unique_ptr<vtkCGALHelper::Vespa_surface> cgalSurface =
    std::make_unique<vtkCGALHelper::Vespa_surface>();

  // CGAL Processing
  // ---------------
  // TODO: add fields data

  bool isSurface = false;

  // Tries to convert the soup into a surface
  try
  {
    if (this->AttemptRepair)
    {
      pmp::repair_polygon_soup(cgalSoup->points, cgalSoup->faces);
      if (!pmp::orient_polygon_soup(cgalSoup->points, cgalSoup->faces))
      {
        vtkWarningMacro("Failed to orient the polygon soup correctly.");
        return 0;
      }
    }

    isSurface = pmp::is_polygon_soup_a_polygon_mesh(cgalSoup->faces);
    if (isSurface)
    {
      pmp::polygon_soup_to_polygon_mesh(cgalSoup->points, cgalSoup->faces, cgalSurface->surface);
    }
    else
    {
      vtkWarningMacro(
        "Warning: polygon soup does not describe a polygon mesh."
        "You may want to try the VESPA Alpha Wrapping filter if you need a manifold mesh.");
    }
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception during soup processing: " << e.what());
    return 0;
  }

  // Now that we have a surface, inspect its properties
  try
  {
    if (isSurface && this->CheckWatertight)
    {
      bool closed = CGAL::is_closed(cgalSurface->surface);
      if (!closed)
      {
        vtkWarningMacro("Input is not closed.");
        if (this->AttemptRepair)
        {
          // collect one halfedge per boundary cycle
          std::vector<Graph_halfedge> borderCycles;
#if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(6,2,0)
          CGAL::extract_boundary_cycles(cgalSurface->surface, std::back_inserter(borderCycles));
#else
          pmp::extract_boundary_cycles(cgalSurface->surface, std::back_inserter(borderCycles));
#endif

          std::vector<Graph_Verts> patch_vertices;
          std::vector<Graph_Faces> patch_facets;
          // fill boundary cycles
          for (Graph_halfedge h : borderCycles)
          {
            pmp::triangulate_refine_and_fair_hole(cgalSurface->surface, h,
              pmp::parameters::fairing_continuity(0)
                .face_output_iterator(std::back_inserter(patch_facets))
                .vertex_output_iterator(std::back_inserter(patch_vertices)));
          }

          // check reparation
          closed = CGAL::is_closed(cgalSurface->surface);
          vtkWarningMacro("Closing " << (closed ? "successful." : "failed."));
        }
      }

      if (closed) // may have been repaired
      {
        bool volume = CGAL::Polygon_mesh_processing::does_bound_a_volume(cgalSurface->surface);
        if (!volume)
        {
          vtkWarningMacro("Input is not watertight.");
          if (this->AttemptRepair)
          {
            pmp::orient_to_bound_a_volume(cgalSurface->surface);

            // check reparation
            volume = CGAL::Polygon_mesh_processing::does_bound_a_volume(cgalSurface->surface);
            vtkWarningMacro("Re-orientation " << (volume ? "successful." : "failed."));
          }
        }
      }
    }

    if (isSurface && this->CheckIntersect)
    {
      bool intersect = CGAL::Polygon_mesh_processing::does_self_intersect(cgalSurface->surface);
      if (intersect)
      {
        vtkWarningMacro("Self intersection detected");
        if (this->AttemptRepair)
        {
          pmp::experimental::autorefine_and_remove_self_intersections(
            cgalSurface->surface, pmp::parameters::preserve_genus(false));

          // check reparation
          intersect = CGAL::Polygon_mesh_processing::does_self_intersect(cgalSurface->surface);
          vtkWarningMacro("Remove intersection " << (intersect ? "successful." : "failed."));
        }
      }
    }
  }
  catch (std::exception& e)
  {
    vtkErrorMacro("CGAL Exception during surface processing: " << e.what());
    return 0;
  }

  // VTK output
  if (this->AttemptRepair)
  {
    if (isSurface)
    {
      vtkCGALHelper::toVTK(cgalSurface.get(), output);
      this->interpolateAttributes(input, output);
    }
    else
    {
      vtkCGALHelper::toVTK(cgalSoup.get(), output);
      this->interpolateAttributes(input, output);
    }
  }
  else
  {
    output->ShallowCopy(input);
  }

  return 1;
}
