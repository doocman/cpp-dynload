# cpp-dynload

Low level abstraction library for cross-platform dynamic library loading. Complete with RAII and as much type-safety as
can be hoped for in this context.

The library is designed so that users should never need to perform any `reinterpret_cast` or any pointer-to-pointer
casts.

Currently, the library's main purpose is to provide a cross-platform way to load dynamic libraries (`.dll` on windows,
`.so` on *NIX platforms) with added type safety.

## Usage

Say that we want to create the dynamic library `foo` like:
```c++

int foo(int arg1, int arg2) {
    return arg1 + arg2;
}

```

which we want to use. Then `DynamicLoad` should be used to "export" the functions in `foo` like: 
```c++

#include <dynl/dynl_export.hpp>

DYNLOAD_EXPORT_C int foo(int arg1, int arg2) {
    return arg1 + arg2;
}

```

The `DYNLOAD_EXPORT_C` adds the `extern "C"` to use the naming conventions of C to keep function names consistent
across different compilers. `DYNLOAD_EXPORT` can be used if dynamic libraries and host are always built by the same
compiler (same vendor, version and settings), but you must also find the mangled name of the function.

Then we can import the library and function in a different software as:

```c++

#include <dynl/dynl.hpp>

#include <iostream>
#include <type_traits>

void main() {
    dynl::dynamic_library lib("/path/to/libfoo.so"); // or "C:\path\to\foo.dll"
    using foo_func_t = std::add_pointer_t<int(int, int)>;
    auto* foo_func_ptr = find_function<foo_func_t>(lib, "foo" /*name of the function*/);
    std::cout << "foo(1, 2) gives " << foo_func_ptr(1, 2) << '\n';
}

```
