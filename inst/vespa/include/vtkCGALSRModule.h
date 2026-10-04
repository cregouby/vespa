
#ifndef VTKCGALSR_EXPORT_H
#define VTKCGALSR_EXPORT_H

#ifdef VTKCGALSR_STATIC_DEFINE
#  define VTKCGALSR_EXPORT
#  define VTKCGALSR_NO_EXPORT
#else
#  ifndef VTKCGALSR_EXPORT
#    ifdef vtkCGALSR_EXPORTS
        /* We are building this library */
#      define VTKCGALSR_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKCGALSR_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKCGALSR_NO_EXPORT
#    define VTKCGALSR_NO_EXPORT 
#  endif
#endif

#ifndef VTKCGALSR_DEPRECATED
#  define VTKCGALSR_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKCGALSR_DEPRECATED_EXPORT
#  define VTKCGALSR_DEPRECATED_EXPORT VTKCGALSR_EXPORT VTKCGALSR_DEPRECATED
#endif

#ifndef VTKCGALSR_DEPRECATED_NO_EXPORT
#  define VTKCGALSR_DEPRECATED_NO_EXPORT VTKCGALSR_NO_EXPORT VTKCGALSR_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKCGALSR_NO_DEPRECATED
#    define VTKCGALSR_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkCGALSRModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKCGALSR_EXPORT_H */
