//
// Created by rvons on 2023-06-26.
//

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <gmock/gmock.h>

#include <dynltest/cross_test.hpp>

#include <dynl/dynl.hpp>
#include <dynl/dynl_except.hpp>

namespace dynltest {
using namespace ::testing;
using namespace ::dynl;

char const *lib_path{};

class DynLoadTest : public ::testing::Test {
protected:
  dynl::dynamic_library lib{};

public:
  DynLoadTest() : lib(lib_path) {}
};

TEST_F(DynLoadTest, ImportSimpleFunction) // NOLINT
{
  auto foostr = find_function<char const *()>(lib, "foostr");
  auto foostr_res = foostr();
  EXPECT_THAT(foostr_res, StrEq("foo"));
}
TEST_F(DynLoadTest, SizeOfInts) // NOLINT
{
  EXPECT_THAT(std::invoke(find_function<std::size_t()>(lib, "sizeof_size_t")),
              Eq(sizeof(std::size_t)));
  EXPECT_THAT(
      std::invoke(find_function<std::size_t()>(lib, "sizeof_int_least8_t")),
      Eq(sizeof(std::int_least8_t)));
  EXPECT_THAT(
      std::invoke(find_function<std::size_t()>(lib, "sizeof_int_least16_t")),
      Eq(sizeof(std::int_least16_t)));
  EXPECT_THAT(
      std::invoke(find_function<std::size_t()>(lib, "sizeof_int_least32_t")),
      Eq(sizeof(std::int_least32_t)));
  EXPECT_THAT(
      std::invoke(find_function<std::size_t()>(lib, "sizeof_int_least64_t")),
      Eq(sizeof(std::int_least64_t)));
}
TEST_F(DynLoadTest, ReturnCustomType) // NOLINT
{
  EXPECT_THAT(
      std::invoke(find_function<std::size_t()>(lib, "sizeof_dynl_test_struct")),
      Eq(sizeof(test_struct)));
  ASSERT_THAT(std::invoke(find_function<std::size_t()>(lib, "offset_data1")),
              Eq(offsetof(test_struct, data1)));
  ASSERT_THAT(std::invoke(find_function<std::size_t()>(lib, "offset_data2")),
              Eq(offsetof(test_struct, data2)));
  ASSERT_THAT(std::invoke(find_function<std::size_t()>(lib, "offset_data3")),
              Eq(offsetof(test_struct, data3)));
  ASSERT_THAT(std::invoke(find_function<std::size_t()>(lib, "offset_data4")),
              Eq(offsetof(test_struct, data4)));
  ASSERT_THAT(std::invoke(find_function<std::size_t()>(lib, "offset_data5")),
              Eq(offsetof(test_struct, data5)));
  ASSERT_THAT(std::invoke(find_function<std::size_t()>(lib, "offset_data6")),
              Eq(offsetof(test_struct, data6)));
  auto ts =
      std::invoke(find_function<test_struct()>(lib, "create_test_struct"));
  EXPECT_THAT(ts.data1, Eq(1));
  EXPECT_THAT(ts.data2, Eq('2'));
  EXPECT_THAT(ts.data3, DoubleEq(3.0));
  EXPECT_THAT(ts.data4, ElementsAre('4', '5', '6'));
  EXPECT_THAT(ts.data5, Eq(7LL));
  EXPECT_THAT(ts.data6, Eq('8'));
}
} // namespace dynltest

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  if (argc != 3) {
    std::cerr << "Bad input arguments! Use 'IMPORTFILE' 'INFOSTR'\n"
              << argc << '\n';
    for (int i = 0; i < argc; ++i) {
      std::cerr << argv[i] << '\n';
    }
    return EXIT_FAILURE;
  }
  dynltest::lib_path = argv[1];
  std::cout << "libpath: " << dynltest::lib_path << '\n';
  return RUN_ALL_TESTS();
}
