
#ifndef VTKCGALDELAUNAY_EXPORT_H
#define VTKCGALDELAUNAY_EXPORT_H

#ifdef VTKCGALDELAUNAY_STATIC_DEFINE
#  define VTKCGALDELAUNAY_EXPORT
#  define VTKCGALDELAUNAY_NO_EXPORT
#else
#  ifndef VTKCGALDELAUNAY_EXPORT
#    ifdef vtkCGALDelaunay_EXPORTS
        /* We are building this library */
#      define VTKCGALDELAUNAY_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKCGALDELAUNAY_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKCGALDELAUNAY_NO_EXPORT
#    define VTKCGALDELAUNAY_NO_EXPORT 
#  endif
#endif

#ifndef VTKCGALDELAUNAY_DEPRECATED
#  define VTKCGALDELAUNAY_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKCGALDELAUNAY_DEPRECATED_EXPORT
#  define VTKCGALDELAUNAY_DEPRECATED_EXPORT VTKCGALDELAUNAY_EXPORT VTKCGALDELAUNAY_DEPRECATED
#endif

#ifndef VTKCGALDELAUNAY_DEPRECATED_NO_EXPORT
#  define VTKCGALDELAUNAY_DEPRECATED_NO_EXPORT VTKCGALDELAUNAY_NO_EXPORT VTKCGALDELAUNAY_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKCGALDELAUNAY_NO_DEPRECATED
#    define VTKCGALDELAUNAY_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkCGALDelaunayModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKCGALDELAUNAY_EXPORT_H */
