**🔧 Fixed Bugs**
- Fixed a bug where `buffer_alignment_is_enough` rejecting pointer-to-member types on MSVC, where `sizeof(T) % alignof(T) != 0`. (#195)
- Fixed a bug where `_HAS_EXCEPTIONS` in MSVC could not affect `EMBED_CXX_ENABLE_EXCEPTION`. (#196)
- Fixed a bug where `make_fn` fail to deduce the correct size of the buffer. (#197)
- Fixed a bug where the noexcept constraint judgment in the in-place constructor was incorrect. (#199)
- Fixed undefined behavior when constructing `fn_ref` from a function pointer. (#200)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Removed the MSVC SAL `_Notnull_` annotation on function pointer constructor parameters. (#201)
- The compile-fail test suite now includes a must-pass guard. (#201)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
