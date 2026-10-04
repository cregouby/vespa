
#ifndef VTKCGALPMP_EXPORT_H
#define VTKCGALPMP_EXPORT_H

#ifdef VTKCGALPMP_STATIC_DEFINE
#  define VTKCGALPMP_EXPORT
#  define VTKCGALPMP_NO_EXPORT
#else
#  ifndef VTKCGALPMP_EXPORT
#    ifdef vtkCGALPMP_EXPORTS
        /* We are building this library */
#      define VTKCGALPMP_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKCGALPMP_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKCGALPMP_NO_EXPORT
#    define VTKCGALPMP_NO_EXPORT 
#  endif
#endif

#ifndef VTKCGALPMP_DEPRECATED
#  define VTKCGALPMP_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKCGALPMP_DEPRECATED_EXPORT
#  define VTKCGALPMP_DEPRECATED_EXPORT VTKCGALPMP_EXPORT VTKCGALPMP_DEPRECATED
#endif

#ifndef VTKCGALPMP_DEPRECATED_NO_EXPORT
#  define VTKCGALPMP_DEPRECATED_NO_EXPORT VTKCGALPMP_NO_EXPORT VTKCGALPMP_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKCGALPMP_NO_DEPRECATED
#    define VTKCGALPMP_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkCGALPMPModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKCGALPMP_EXPORT_H */
