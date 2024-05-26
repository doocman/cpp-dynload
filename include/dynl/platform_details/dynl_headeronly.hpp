///@file
/// Contains dispatch for header-only routine calls

#pragma once

#if defined(DYNL_WINDOWS)
#include <dynl/platform_details/dynl_win32.hpp>
#elif defined(DYNL_UNIX)
#include <dynl/platform_details/dynl_unix.hpp>
#endif

namespace dynl::_backend {

inline namespace {
dynamic_function_symbol do_find_function(dynamic_library_pointer *lib,
                                         char const *name,
                                         error_callback const &ecb) {
  return hh_find_function(lib, name, ecb);
}

dynamic_library_pointer *do_load_library(char const *p,
                                         error_callback const &ecb) {
  return hh_load_library(p, ecb);
}

void do_release_library(dynamic_library_pointer *p) { hh_release_library(p); }
} // namespace
} // namespace dynl::_backend
