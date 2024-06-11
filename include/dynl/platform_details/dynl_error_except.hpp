///@file
/// Contains default error handling routines when exceptions are enabled.
///

#pragma once

#include <dynl/dynl_error.hpp>
#include <dynl/dynl_except.hpp>

namespace dynl {

inline void _default_error_callback(dynl_error const &err) {
  throw dynl_except(err);
}
} // namespace dynl
