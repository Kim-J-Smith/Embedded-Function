**🔧 Fixed Bugs**
- Fixed a bug where `buffer_alignment_is_enough` rejecting pointer-to-member types on MSVC, where `sizeof(T) % alignof(T) != 0`.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
