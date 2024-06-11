///@file
/// Contains dispatch for windows-specific calls.

#pragma once

#include <dynl/dynl_types.hpp>

#include <Windows.h>

namespace dynl::_backend {
inline namespace {
dynamic_library_pointer *placeholder(HMODULE in) {
  return reinterpret_cast<dynamic_library_pointer *>(in);
}
HMODULE native_lib(dynamic_library_pointer *in) {
  return reinterpret_cast<HMODULE>(in);
}
} // namespace
dynamic_function_symbol hh_find_function(dynamic_library_pointer *lib,
                                         char const *name,
                                         error_callback const &ecb) {
  if (auto *f = GetProcAddress(native_lib(lib), name)) {
    return dynamic_function_symbol(
        reinterpret_cast<std::add_pointer_t<void()>>(f));
  } else {
    ecb(dynl_ec::symbol_not_found);
    return nullptr;
  }
}

dynamic_library_pointer *hh_load_library(char const *p,
                                         error_callback const &ecb) {
  if (auto *l = LoadLibraryA(p); l != nullptr) {
    return placeholder(l);
  } else {
    auto winerr = GetLastError();
    switch (winerr) {
    case ERROR_MOD_NOT_FOUND:
      ecb(dynl_ec::library_not_found);
      break;
    case ERROR_INVALID_PARAMETER:
      ecb(dynl_ec::bad_input_parameter);
      break;
    default:
      ecb(dynl_ec::unspecified_error);
      break;
    }
    return nullptr;
  }
}

void hh_release_library(dynamic_library_pointer *p) {
  FreeLibrary(native_lib(p));
}
} // namespace dynl::_backend
