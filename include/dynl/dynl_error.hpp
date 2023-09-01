//
// Created by rvons on 2023-06-25.
//

#ifndef DYNAMICLOAD_DYNL_ERROR_HPP
#define DYNAMICLOAD_DYNL_ERROR_HPP

#include <concepts>
#include <exception>
#include <functional>
#include <string>
#include <string_view>
#include <utility>

namespace dynl {

enum class dynl_ec {
  unspecified_error = -1,
  symbol_not_found = 0,
  library_not_found,
  bad_input_parameter,
};

[[nodiscard]] constexpr std::string_view message(dynl_ec ec) {
  switch (ec) {
  case dynl_ec::unspecified_error:
    return "Unspecified error";
  case dynl_ec::symbol_not_found:
    return "Symbol not found";
  case dynl_ec::library_not_found:
    return "Library file not found";
  case dynl_ec::bad_input_parameter:
    return "Bad input argument";
  default:
    return "UNKNOWN";
  }
}
[[nodiscard]] constexpr char const *cstr_message(dynl_ec ec) {
  return message(ec).data();
}

class dynl_error {
  dynl_ec error_;

public:
  constexpr explicit(false) dynl_error(dynl_ec err) noexcept : error_(err) {}
  [[nodiscard]] constexpr dynl_ec error_code() const noexcept { return error_; }
};

[[nodiscard]] constexpr std::string_view message(dynl_error const &err) {
  return message(err.error_code());
}
[[nodiscard]] constexpr char const *cstr_message(dynl_error const &err) {
  return cstr_message(err.error_code());
}

class error_callback {
  static constexpr auto noop_func = [](void *, dynl_error const &) {};
  void *context_{};
  std::add_pointer_t<void(void *, dynl_error const &)> func_ = noop_func;
  static constexpr void *void_ptr_cast(auto &in) {
    // this const-cast is safe as we re-apply the constness in the cast in
    // func_;
    return static_cast<void *>(
        &const_cast<std::remove_cvref_t<decltype(in)> &>(in));
  }
  template <typename T>
  constexpr explicit error_callback(T &&t, int)
      : context_(void_ptr_cast(t)), func_([](void *in, dynl_error const &err) {
          std::invoke(*static_cast<std::remove_reference_t<T> *>(in), err);
        }) {}

public:
  template <typename T>
    requires(std::invocable<T, dynl_error const &> &&
             !std::is_rvalue_reference_v<T &&>)
  constexpr explicit(false) error_callback(T &&t)
      : error_callback(std::forward<T>(t), 0) {}

  template <typename T>
    requires(
        std::invocable<T, dynl_error const &> &&
        std::convertible_to<T, std::add_pointer_t<void(dynl_error const &)>>)
  constexpr explicit(false) error_callback(T const &t)
      : error_callback(
            static_cast<std::add_pointer_t<void(dynl_error const &)>>(t), 0) {}

  void operator()(dynl_error const &err) const {
    std::invoke(func_, context_, err);
  }
};
} // namespace dynl

#endif // DYNAMICLOAD_DYNL_ERROR_HPP
