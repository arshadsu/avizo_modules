// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.
#ifndef SONIFYSPATIAL_API_EXPORT_MACRO_H
#define SONIFYSPATIAL_API_EXPORT_MACRO_H

#ifdef SONIFYSPATIAL_STATIC
#   define SONIFYSPATIAL_API
#else
#   ifdef _WIN32
#       ifdef SONIFYSPATIAL_EXPORTS
#           define SONIFYSPATIAL_API __declspec(dllexport)
#       else
#           define SONIFYSPATIAL_API __declspec(dllimport)
#       endif
#   else
#       define SONIFYSPATIAL_API __attribute__ ((visibility("default")))
#   endif
#endif

#endif
