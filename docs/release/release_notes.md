**🔧 Fixed Bugs**
- Fixed a double-free bug by reordering base classes of `lifetime_operations_impl`. (#189)
- Fixed a bug where treating non-trivial functor as *stateless*.
- Fixed a double-free bug in `swap()` when a move constructor throws.
- Fixed undefined behavior caused by reading inactive members of the `ErasurePass` union.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Added tests for throwing copy/move constructors and assignments. (#189)
- Added tests for the `swap()` exception-safety fix.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
