**🔧 Fixed Bugs**
- Fixed a double-free bug by reordering base classes of `lifetime_operations_impl`. (#189)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Added tests for throwing copy/move constructors and assignments. (#189)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
