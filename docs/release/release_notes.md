**🔧 Fixed Bugs**
- Fixed non-owning wrappers calling an incorrect overload: the functor type is no longer decay-copied, so cv-qualification is preserved during invocation. (#213)
- Fixed an improper use of `constexpr` in internal function. (#215)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Added `[[clang::lifetimebound]]` to the constructor of non-owning wrappers (`fn_ref`), so that clang can diagnose dangling reference in more cases. (#217)

**🛠️ Optimizations and Improvements**
- Added GCC-15 and GCC-16 to CI. (#216)
- Added conformance tests for the lifetimebound diagnostics. (#217)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
