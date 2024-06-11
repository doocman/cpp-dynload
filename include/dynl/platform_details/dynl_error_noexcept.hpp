///@file
/// Contains default error handling routines when exceptions are disabled.
///

#pragma once

#include <cstdlib>
#include <iostream>

#include <dynl/dynl_error.hpp>

namespace dynl {

inline void _default_error_callback(dynl_error const &err) {
  std::cerr << "Dynload error: " << message(err) << " (" << details(err)
            << ")\n";
  std::abort();
}
} // namespace dynl
