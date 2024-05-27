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
#include <dynl/dynl_types.hpp>

#if defined(DYNLOAD_HEADERONLY)
#include <dynl/platform_details/dynl_headeronly.hpp>
#else
#include <dynl/platform_details/dynl_src.hpp>
#endif

namespace dynl {

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

  constexpr dynamic_library() = default;

  /// Extract function from dynamic library.
  /// \tparam T Function signature
  /// \param in Library to extract function from
  /// \param name Name of function (use 'extern "C"' for non-mangled name)
  /// \param ecb Error handler. Default throws exception if enabled, otherwise
  /// aborts with error message.
  /// \return Function pointer. May be null if ecb returns without throwing.
  template <typename T>
  c_function_pointer<T>
  find_function(char const *name,
                error_callback const &ecb = {_default_error_callbacker}) const {
    auto res = _backend::do_find_function(lib_.get(), name, ecb);
    return symbol_cast<T>(res);
  }
  [[nodiscard]] bool valid() const noexcept { return lib_ != nullptr; }
  explicit operator bool() const noexcept { return valid(); }
};

template <typename T>
c_function_pointer<T>
find_function(dynamic_library const &in, char const *name,
              error_callback const &ecb = {_default_error_callbacker}) {
  return in.template find_function<T>(name, ecb);
}

} // namespace dynl
