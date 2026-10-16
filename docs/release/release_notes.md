**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- All owning function wrappers (`fn`, `unique_fn`, etc.) now reject callable objects whose destructor may throw exceptions. (#221)
- Deducing-this (C++23) support now additionally requires the compiler to define `__cpp_explicit_this_parameter`; previously a C++23 language mode alone was enough. (#226)
- `noexcept` function type (C++17) support now additionally requires the compiler to define `__cpp_noexcept_function_type`; previously a C++17 language mode alone was enough. (#226)

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- The destructor of all owning function wrappers are declared as `noexcept` now. (#221)
- Documented the usage of CMake integration in `README.md`. (#223)
- `-Wundef` (GCC/Clang) and C4668 (MSVC) warnings have been resolved. (#226)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
- `CMakeLists.txt` and `cmake/` are added to `header_only.zip`.
