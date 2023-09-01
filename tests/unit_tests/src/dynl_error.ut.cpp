//
// Created by rvons on 2023-09-01.
//

#include <dynl/dynl_error.hpp>

#include <gmock/gmock.h>

namespace dynl {
TEST(DynlError, CompileTest) {
  (void)error_callback([](dynl_error const &) {});
}

} // namespace dynl
