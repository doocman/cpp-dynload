# cpp-dynload

Low level abstraction library for cross-platform dynamic library loading. Complete with RAII and as much type-safety as
can be hoped for in this context.

The library is designed so that users should never need to perform any `reinterpret_cast` or any pointer-to-pointer
casts.
