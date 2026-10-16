**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- All owning function wrappers (`fn`, `unique_fn`, etc.) now reject callable objects whose destructor may thow exceptions.
- Deducing-this (C++23) support now additionally requires the compiler to define `__cpp_explicit_this_parameter`; previously a C++23 language mode alone was enough.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- The destructor of all owning function wrappers are declare as `noexcept` now.
- Documented the usage of CMake integration in `README.md`.
- `-Wundef` (GCC/Clang) and C4668 (MSVC) warnings have been resolved.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
