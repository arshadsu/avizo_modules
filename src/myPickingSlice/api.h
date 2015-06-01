// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.
#ifndef MYPICKINGSLICE_API_EXPORT_MACRO_H
#define MYPICKINGSLICE_API_EXPORT_MACRO_H

#ifdef MYPICKINGSLICE_STATIC
#   define MYPICKINGSLICE_API
#else
#   ifdef _WIN32
#       ifdef MYPICKINGSLICE_EXPORTS
#           define MYPICKINGSLICE_API __declspec(dllexport)
#       else
#           define MYPICKINGSLICE_API __declspec(dllimport)
#       endif
#   else
#       define MYPICKINGSLICE_API __attribute__ ((visibility("default")))
#   endif
#endif

#endif
