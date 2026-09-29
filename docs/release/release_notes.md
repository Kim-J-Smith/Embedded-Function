**🔧 Fixed Bugs**
- Fixed the bug that occurred when using the non-existent `__builtin_launder` in GCC5 and GCC6. (#185)
- Added workarounds for MSVC 19.10 ~ 19.26 and ICC 16 ~ 19. (#185)
- Fixed a bug where calling `clear()` after an assignment that throws an exception would trigger double freeing. (#187)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Added a benchmark for the NTTP bind (`std::constant_wrapper`) feature. (#186)

**🛠️ Optimizations and Improvements**
- Adjusted the style of some internal traits. (#184)
- `std::is_default_constructible<ebd::fn_ref<...>>` and `std::is_constructible<ebd::fn_ref<...>, std::nullptr_t>` now yield `false` starting from C++17, instead of requiring C++20. (#185)
- Did some small internal refactoring to improve maintainability. (#185)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
- GCC 16.0 defines `__cpp_lib_constant_wrapper` to a value below `202603L`, so the library keeps this feature disabled there. Users of GCC 16.0 can opt in by defining `__cpp_lib_constant_wrapper=202603L` before including the header; the test suite and benchmarks apply this workaround in `test/__constant_wrapper.hpp`.
