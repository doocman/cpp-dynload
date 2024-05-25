///@file
/// Contains dispatch for non-header-only calls.

#pragma once

#include <dynl/dynl_types.hpp>

namespace dynl::_backend {
dynamic_function_symbol src_find_function(dynamic_library_pointer *lib,
                                         char const *name,
                                         error_callback const &ecb);

dynamic_library_pointer *src_load_library(char const *p,
                                         error_callback const &ecb);

void src_release_library(dynamic_library_pointer *p);

inline namespace {
dynamic_function_symbol do_find_function(dynamic_library_pointer *lib,
                                         char const *name,
                                         error_callback const &ecb) {
  return src_find_function(lib, name, ecb);
}

dynamic_library_pointer *do_load_library(char const *p,
                                         error_callback const &ecb) {
  return src_load_library(p, ecb);
}

void do_release_library(dynamic_library_pointer *p) { src_release_library(p); }
} // namespace
} // namespace dynl::_backend