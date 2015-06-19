#pragma once
#ifndef __CPPFX_STL_EXPORT_H
#define __CPPFX_STL_EXPORT_H
#include <cppfx/config.h>
#include <cppfx/export.h>
#  ifdef CPPFX_STATIC
#    define CPPFX_STL
#  else
#    ifdef cppfx_EXPORTS
#      define CPPFX_STL
#    else
#      define CPPFX_STL extern
#    endif
#  endif
#endif