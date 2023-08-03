//
// Created by rvons on 2023-06-26.
//

#include <dynltest/cross_test.hpp>

#include <dynl/dynl_export.hpp>

#include <cstddef>
#include <cstdint>

extern "C" {
DYNLOAD_EXPORT char const *foostr() { return "foo"; }
DYNLOAD_EXPORT std::size_t sizeof_size_t() { return sizeof(std::size_t); }
DYNLOAD_EXPORT std::size_t sizeof_int_least8_t() {
  return sizeof(std::int_least8_t);
}
DYNLOAD_EXPORT std::size_t sizeof_int_least16_t() {
  return sizeof(std::int_least16_t);
}
DYNLOAD_EXPORT std::size_t sizeof_int_least32_t() {
  return sizeof(std::int_least32_t);
}
DYNLOAD_EXPORT std::size_t sizeof_int_least64_t() {
  return sizeof(std::int_least64_t);
}

DYNLOAD_EXPORT std::size_t sizeof_dynl_test_struct() {
  return sizeof(dynltest::test_struct);
}
DYNLOAD_EXPORT std::size_t offset_data1() {
  return offsetof(dynltest::test_struct, data1);
}
DYNLOAD_EXPORT std::size_t offset_data2() {
  return offsetof(dynltest::test_struct, data2);
}
DYNLOAD_EXPORT std::size_t offset_data3() {
  return offsetof(dynltest::test_struct, data3);
}
DYNLOAD_EXPORT std::size_t offset_data4() {
  return offsetof(dynltest::test_struct, data4);
}
DYNLOAD_EXPORT std::size_t offset_data5() {
  return offsetof(dynltest::test_struct, data5);
}
DYNLOAD_EXPORT std::size_t offset_data6() {
  return offsetof(dynltest::test_struct, data6);
}
DYNLOAD_EXPORT dynltest::test_struct create_test_struct() {
  return dynltest::test_struct{1, '2', 3., {'4', '5', '6'}, 7LL, '8'};
}
}
