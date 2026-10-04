
#ifndef VTKCGALALGORITHM_EXPORT_H
#define VTKCGALALGORITHM_EXPORT_H

#ifdef VTKCGALALGORITHM_STATIC_DEFINE
#  define VTKCGALALGORITHM_EXPORT
#  define VTKCGALALGORITHM_NO_EXPORT
#else
#  ifndef VTKCGALALGORITHM_EXPORT
#    ifdef vtkCGALAlgorithm_EXPORTS
        /* We are building this library */
#      define VTKCGALALGORITHM_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKCGALALGORITHM_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKCGALALGORITHM_NO_EXPORT
#    define VTKCGALALGORITHM_NO_EXPORT 
#  endif
#endif

#ifndef VTKCGALALGORITHM_DEPRECATED
#  define VTKCGALALGORITHM_DEPRECATED __declspec(deprecated)
#endif

#ifndef VTKCGALALGORITHM_DEPRECATED_EXPORT
#  define VTKCGALALGORITHM_DEPRECATED_EXPORT VTKCGALALGORITHM_EXPORT VTKCGALALGORITHM_DEPRECATED
#endif

#ifndef VTKCGALALGORITHM_DEPRECATED_NO_EXPORT
#  define VTKCGALALGORITHM_DEPRECATED_NO_EXPORT VTKCGALALGORITHM_NO_EXPORT VTKCGALALGORITHM_DEPRECATED
#endif

/* NOLINTNEXTLINE(readability-avoid-unconditional-preprocessor-if) */
#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKCGALALGORITHM_NO_DEPRECATED
#    define VTKCGALALGORITHM_NO_DEPRECATED
#  endif
#endif

/* VTK-HeaderTest-Exclude: vtkCGALAlgorithmModule.h */

/* Include ABI Namespace */
#include "vtkABINamespace.h"

#endif /* VTKCGALALGORITHM_EXPORT_H */
