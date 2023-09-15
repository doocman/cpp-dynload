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
constexpr bool operator==(dynl_error lhs, dynl_error rhs) noexcept {
  return lhs.error_code() == rhs.error_code();
}

namespace details {
class error_callback {
  using f_ptr_t = std::add_pointer_t<void(dynl_error const &)>;
  union cb_t {
    void *data_{};
    f_ptr_t f_ptr_;
  };
  static constexpr auto noop_func = [](cb_t, dynl_error const &) {};
  cb_t context_{};
  std::add_pointer_t<void(cb_t, dynl_error const &)> func_ = noop_func;
  static constexpr cb_t void_ptr_cast(auto *in) {
    // this const-cast is safe as we re-apply the constness in the cast in
    // func_;
    return cb_t{.data_ = reinterpret_cast<void *>(in)};
  }
  static constexpr cb_t
  void_ptr_cast(std::add_pointer_t<void(dynl_error const &)> in) {
    return cb_t{.f_ptr_ = in};
  }
  template <typename T>
  constexpr explicit error_callback(T *t, int)
      : context_(void_ptr_cast(t)), func_([](cb_t in, dynl_error const &err) {
          if constexpr (std::is_same_v<f_ptr_t, T *>) {
            std::invoke(in.f_ptr_, err);
          } else {
            std::invoke(*static_cast<T *>(in.data_), err);
          }
        }) {}

public:
  template <typename T>
    requires(std::invocable<T, dynl_error const &> &&
             !std::is_rvalue_reference_v<T &&> &&
             !std::is_same_v<std::remove_cvref_t<T>, error_callback>)
  constexpr explicit(false) error_callback(T &&t) : error_callback(&t, 0) {}

  template <typename T>
    requires(
        std::invocable<T, dynl_error const &> &&
        std::convertible_to<T, std::add_pointer_t<void(dynl_error const &)>> &&
        !std::is_same_v<std::remove_cvref_t<T>, error_callback>)
  constexpr explicit(false) error_callback(T const &t)
      : error_callback(static_cast<f_ptr_t>(t), 0) {}

  void operator()(dynl_error const &err) const {
    std::invoke(func_, context_, err);
  }
};
} // namespace details
} // namespace dynl

#endif // DYNAMICLOAD_DYNL_ERROR_HPP
