//
// Created by rvons on 2023-06-25.
//

#include <dynl/dynl.hpp>

#if DYNLOAD_NO_EXCEPTIONS
#include <cstdlib>
#include <iostream>
#else
#include <dynl/dynl_except.hpp>
#endif
namespace dynl {

void _default_error_callback(dynl_error const &err) {
#if DYNLOAD_NO_EXCEPTIONS
  std::cerr << "Dynload error: " << message(err) << " (" << details(err)
            << ")\n";
  std::abort();
#else
  throw dynl_except(err);
#endif
}

} // namespace dynl
