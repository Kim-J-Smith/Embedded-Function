**🔧 Fixed Bugs**
- Fixed the bug that occurred when using the non-existent `__builtin_launder` in GCC5 and GCC6. (#185)
- Added workarounds for MSVC 19.10 ~ 19.26 and ICC 16 ~ 19. (#185)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Added a benchmark for the experimental NTTP bind (`std::constant_wrapper`) feature. (#186)

**🛠️ Optimizations and Improvements**
- Adjusted the style of some internal traits. (#184)
- Did some small internal refactoring to improve maintainability. (#185)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
