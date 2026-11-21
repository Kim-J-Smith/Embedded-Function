**🔧 Fixed Bugs**
- Fixed a compilation failure on Clang 3.7 ~ 23.1 where Clang's `__is_complete_type` builtin could report a complete type as incomplete, causing declarations such as `ebd::fn<void(A<int>)>` to fail to compile. The builtin is no longer used; the portable `sizeof`-based completeness check is used instead.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- None.
