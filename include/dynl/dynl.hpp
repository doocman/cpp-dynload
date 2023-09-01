///@file
/// This file contains low-level abstractions for dynamic loading of library
/// files for the current OS (.dll for Windows, .so for Linux/Unix/BSD/MacOS)
/// Extending this to more OS:es requires a lib that provides an implementation
/// for the following functions:
/// void do_release_library(dynamic_library_pointer *p);
/// dynamic_library_pointer*
///   do_load_library(char const* p, error_callback const&);
/// dynamic_function_symbol*
///   do_find_function(dynamic_library_pointer *lib, char const *name,
///   error_callback const&);

#include <memory>

#include <dynl/dynl_error.hpp>

namespace dynl {
/// \typedef c_function_pointer
/// Template alias to make function pointer API readable.
template <typename T> using c_function_pointer = T *;

void _default_error_callback(dynl_error const &);
constexpr auto _default_error_callbacker = [](dynl_error const &err) {
  _default_error_callback(err);
};

class dynamic_library;
class dynamic_function_symbol;

/// \class dynamic_library_pointer
/// Represents a raw dynamic library. It lacks any formal definition and is only
/// used for type safety. reinterpret_cast it to/from platform-dependent type
/// representing the dynamic library.
class dynamic_library_pointer;
namespace _backend {

/// Platform-dependent low-level dynamic library function access.
/// You must implement your own version of this when extending the code for
/// further platforms. If an error occurs, call err_cb with an appropriate error
/// and return nullptr.
/// \param p Path to library
/// \param err_cb Error handler. Default throws exception if enabled, otherwise
/// aborts with error message.
/// \return Library pointer. Can be null if err_cb returns without throwing.
dynamic_library_pointer *do_load_library(char const *p,
                                         error_callback const &err_cb = {
                                             _default_error_callbacker});

/// Platform-dependent low-level dynamic library function access.
/// You must implement your own version of this when extending the code for
/// further platforms.
/// \param p Pointer to library
void do_release_library(dynamic_library_pointer *p);

/// Platform-dependent low-level dynamic library function access.
/// You must implement your own version of this when extending the code for
/// further platforms. If an error occurs, call err_cb with an appropriate error
/// and return nullptr.
/// \param lib Library to search
/// \param name Function name to find
/// \param err_cb Error handler. Default throws exception if enabled, otherwise
/// aborts with error message.
/// \return Symbol, can be null if callback returns without throwing.
dynamic_function_symbol
do_find_function(dynamic_library_pointer *lib, char const *name,
                 error_callback const &err_cb = {_default_error_callbacker});
} // namespace _backend

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

/// Cast a dynamic_function_symbol to the specified function pointer.
/// Note, the symbol lacks any type information and as such the responsibility
/// of correct usage lies on the user.
/// \tparam T Function pointer type (e.g. void(), bool(int))
/// \param in Symbol to convert
/// \return Function pointer from the symbol
template <typename T>
inline c_function_pointer<T> symbol_cast(dynamic_function_symbol in) {
  return reinterpret_cast<c_function_pointer<T>>(in.raw_symbol_);
}

struct library_unloader {
  void operator()(dynamic_library_pointer *in) const {
    if (in) {
      _backend::do_release_library(in);
    }
  }
};
using smart_library_pointer =
    std::unique_ptr<dynamic_library_pointer, library_unloader>;

/// \class dynamic_library
/// RAII dynamic library object. Automatically closes itself on end-of-life.
class dynamic_library {
  smart_library_pointer lib_;

public:
  explicit dynamic_library(
      char const *path, error_callback const &ecb = {_default_error_callbacker})
      : lib_(_backend::do_load_library(path, ecb)) {}
  template <typename T>
    requires(std::convertible_to<T &, error_callback>)
  dynamic_library(char const *path, T &&ecb)
      : dynamic_library(path, error_callback(ecb)) {}

  constexpr dynamic_library() = default;

  /// Extract function from dynamic library.
  /// \tparam T Function signature
  /// \param in Library to extract function from
  /// \param name Name of function (use 'extern "C"' for non-mangled name)
  /// \param ecb Error handler. Default throws exception if enabled, otherwise
  /// aborts with error message.
  /// \return Function pointer. May be null if ecb returns without throwing.
  template <typename T>
  friend c_function_pointer<T>
  find_function(dynamic_library const &in, char const *name,
                error_callback const &ecb = {_default_error_callbacker}) {
    auto res = _backend::do_find_function(in.lib_.get(), name, ecb);
    return symbol_cast<T>(res);
  }
  [[nodiscard]] bool valid() const noexcept { return lib_ != nullptr; }
  explicit operator bool() const noexcept { return valid(); }
};

} // namespace dynl
