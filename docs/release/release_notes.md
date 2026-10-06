**🔧 Fixed Bugs**
- Fixed a bug where `buffer_alignment_is_enough` rejecting pointer-to-member types on MSVC, where `sizeof(T) % alignof(T) != 0`.
- Fixed a bug where `_HAS_EXCEPTIONS` in MSVC could not affect `EMBED_CXX_ENABLE_EXCEPTION`.
- Fixed a bug where `make_fn` fail to deduce the correct size of the buffer.
- Fixed a bug where the noexcept constraint judgment in the in-place constructor was incorrect.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
