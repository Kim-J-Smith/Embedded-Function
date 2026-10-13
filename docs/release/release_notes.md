**🔧 Fixed Bugs**
- Fixed non-owning wrappers calling an incorrect overload: the functor type is no longer decay-copied, so cv-qualification is preserved during invocation.
- Fixed an improper use of `constexpr` in internal function.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Added `[[clang::lifetimebound]]` to the constructor of non-owning wrappers (`fn_ref`), so that clang can diagnose dangling reference in more cases.

**🛠️ Optimizations and Improvements**
- Added conformance tests for the lifetimebound diagnostics.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
