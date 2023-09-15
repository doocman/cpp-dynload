//
// Created by rvons on 2023-09-15.
//

#include <dynl/dynl.hpp>

#include <gmock/gmock.h>

namespace dynl {
using namespace ::testing;
TEST(Dynl, DynamicLibCb) // NOLINT
{
  dynl_error err = dynl_ec::unspecified_error;
  EXPECT_NO_THROW(
      dynamic_library lib(" this file should not exist ",
                          [&err](dynl_error err_in) { err = err_in; }));
  EXPECT_THAT(err, Eq(dynl_ec::library_not_found)) << message(err);
}
} // namespace dynl
