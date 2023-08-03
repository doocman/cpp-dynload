//
// Created by rvons on 2023-06-26.
//

#ifndef DYNAMICLOAD_DYNL_EXCEPT_HPP
#define DYNAMICLOAD_DYNL_EXCEPT_HPP

#include <exception>

#include <dynl/dynl_error.hpp>

namespace dynl {
class dynl_except final : public std::exception {
  dynl_error err_;

public:
  explicit dynl_except(dynl_error err) : err_(std::move(err)) {}
  [[nodiscard]] const char *what() const noexcept override {
    return cstr_message(err_);
  }
  [[nodiscard]] std::string_view message() const noexcept {
    return ::dynl::message(err_);
  }
};
} // namespace dynl

#endif // DYNAMICLOAD_DYNL_EXCEPT_HPP
