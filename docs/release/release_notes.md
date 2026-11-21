**🔧 Fixed Bugs**
- Fixed a compilation failure on Clang 3.7 ~ 23.1 where Clang's `__is_complete_type` builtin could report a complete type as incomplete, causing declarations such as `ebd::fn<void(A<int>)>` to fail to compile. The builtin is no longer used; the portable `sizeof`-based completeness check is used instead.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Made the GCC 5.4 CI Docker image more robust when downloading CMake: `curl` is now used instead of `wget`, the GitHub mirror is tried first, and the archive is verified against its official SHA-256.

**📌 Notes**
- None.
