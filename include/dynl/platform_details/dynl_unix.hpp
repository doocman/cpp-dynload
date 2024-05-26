///@file
/// Contains dispatch for unix/linux specific calls.

#pragma once

#include <dynl/dynl_types.hpp>

#include <dlfcn.h>

namespace dynl::_backend {
inline namespace {
dynamic_library_pointer *as_placeholder(void *in) {
  return reinterpret_cast<dynamic_library_pointer *>(in);
}
void *as_native_lib(dynamic_library_pointer *in) {
  return reinterpret_cast<dynamic_library_pointer *>(in);
}

dynamic_function_symbol hh_find_function(dynamic_library_pointer *lib,
                                         char const *name,
                                         error_callback const &ecb) {
  if (auto *handle = dlsym(as_native_lib(lib), name)) {
    return dynamic_function_symbol(
        reinterpret_cast<c_function_pointer<void()>>(handle));
  } else {
    ecb(dynl_ec::symbol_not_found);
    return nullptr;
  }
}

dynamic_library_pointer *hh_load_library(char const *p,
                                         error_callback const &ecb) {
  if (auto *handle = dlopen(p, RTLD_NOW)) {
    return as_placeholder(handle);
  } else {
    ecb(dynl_ec::library_not_found);
    return nullptr;
  }
}

void hh_release_library(dynamic_library_pointer *p) {
  dlclose(as_native_lib(p));
}
} // namespace
} // namespace dynl::_backend