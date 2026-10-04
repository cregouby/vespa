
#ifndef VTKCGALPSP_EXPORT_H
#define VTKCGALPSP_EXPORT_H

#ifdef VTKCGALPSP_STATIC_DEFINE
#  define VTKCGALPSP_EXPORT
#  define VTKCGALPSP_NO_EXPORT
#else
#  ifndef VTKCGALPSP_EXPORT
#    ifdef vtkCGALPSP_EXPORTS
        /* We are building this library */
#      define VTKCGALPSP_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKCGALPSP_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKCGALPSP_NO_EXPORT
#    define VTKCGALPSP_NO_EXPORT 
#  endif
#endif

#ifndef VTKCGALPSP_DEPRECATED
#  define VTKCGALPSP_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKCGALPSP_DEPRECATED_EXPORT
#  define VTKCGALPSP_DEPRECATED_EXPORT VTKCGALPSP_EXPORT VTKCGALPSP_DEPRECATED
#endif

#ifndef VTKCGALPSP_DEPRECATED_NO_EXPORT
#  define VTKCGALPSP_DEPRECATED_NO_EXPORT VTKCGALPSP_NO_EXPORT VTKCGALPSP_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKCGALPSP_NO_DEPRECATED
#    define VTKCGALPSP_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkCGALPSPModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKCGALPSP_EXPORT_H */
