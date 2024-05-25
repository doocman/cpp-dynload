///

#pragma once

#include <dynl/dynl_error.hpp>

#if defined(DYNLOAD_NO_EXCEPTIONS)
#include <dynl/platform_details/dynl_error_noexcept.hpp>
#else
#include <dynl/platform_details/dynl_error_except.hpp>
#endif

namespace dynl {
/// \typedef c_function_pointer
/// Template alias to make function pointer API readable.
template <typename T> using c_function_pointer = T *;

/// \class dynamic_library_pointer
/// Represents a raw dynamic library. It lacks any formal definition and is only
/// used for type safety. reinterpret_cast it to/from platform-dependent type
/// representing the dynamic library.
class dynamic_library_pointer;

constexpr auto _default_error_callbacker = [](dynl_error const &err) {
  _default_error_callback(err);
};

class dynamic_library;

/// \class dynamic_function_symbol
/// Represents a callable symbol from a dynamically loaded library.
/// The symbol lacks any type information and it is up to the user to
/// do the correct 'symbol_cast'.
class dynamic_function_symbol {
public:
  using raw_cymbol_t = c_function_pointer<void()>;

private:
  c_function_pointer<void()> raw_symbol_{};

public:
  explicit constexpr dynamic_function_symbol(raw_cymbol_t rs) noexcept
      : raw_symbol_(rs) {}
  explicit(false) constexpr dynamic_function_symbol(std::nullptr_t) {}
  constexpr dynamic_function_symbol() noexcept = default;

  [[nodiscard]] constexpr bool valid() const noexcept {
    return raw_symbol_ != nullptr;
  }
  constexpr explicit operator bool() const noexcept { return valid(); }

  template <typename T>
  friend c_function_pointer<T> symbol_cast(dynamic_function_symbol in);
};

}

