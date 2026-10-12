**🔧 Fixed Bugs**
- Fixed non-owning wrappers calling an incorrect overload: the functor type is no longer decay-copied, so cv-qualification is preserved during invocation.
- Fixed an improper use of `constexpr` in internal function.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
