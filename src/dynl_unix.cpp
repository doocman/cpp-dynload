//
// Created by rvons on 2023-06-29.
//

#include <dynl/dynl.hpp>

#include <dynl/platform_details/dynl_headeronly.hpp>

namespace dynl::_backend {

dynamic_function_symbol src_find_function(dynamic_library_pointer *lib,
                                         char const *name,
                                         error_callback const &ecb) {
  return hh_find_function(lib, name, ecb);
}

dynamic_library_pointer *src_load_library(char const *p,
                                         error_callback const &ecb) {
  return hh_load_library(p, ecb);
}

void src_release_library(dynamic_library_pointer *p) { hh_release_library(p); }
} // namespace dynl::_backend