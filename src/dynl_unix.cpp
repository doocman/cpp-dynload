//
// Created by rvons on 2023-06-29.
//

#include <dynl/dynl.hpp>

#include <dlfcn.h>

namespace dynl::_backend {
namespace {
dynamic_library_pointer *as_placeholder(void *in) {
  return reinterpret_cast<dynamic_library_pointer *>(in);
}
void *as_native_lib(dynamic_library_pointer *in) {
  return reinterpret_cast<dynamic_library_pointer *>(in);
}
} // namespace

dynamic_function_symbol do_find_function(dynamic_library_pointer *lib,
                                         char const *name,
                                         details::error_callback const &ecb) {
  if (auto *handle = dlsym(as_native_lib(lib), name)) {
    return dynamic_function_symbol(
        reinterpret_cast<c_function_pointer<void()>>(handle));
  } else {
    ecb(dynl_ec::symbol_not_found);
    return nullptr;
  }
}

dynamic_library_pointer *do_load_library(char const *p,
                                         details::error_callback const &ecb) {
  if (auto *handle = dlopen(p, RTLD_NOW)) {
    return as_placeholder(handle);
  } else {
    ecb(dynl_ec::library_not_found);
    return nullptr;
  }
}

void do_release_library(dynamic_library_pointer *p) {
  dlclose(as_native_lib(p));
}
} // namespace dynl::_backend