**🔧 Fixed Bugs**
- Fixed a double-free bug by reordering base classes of `lifetime_operations_impl`. (#189)
- Fixed a bug where treating non-trivial functor as *stateless*.
- Fixed a double-free bug in `swap()` when a move constructor throws.
- Fixed undefined behavior caused by reading inactive members of the `ErasurePass` union.
- Fixed `is_invocable_r` to reject callables that would bind a temporary to a reference return type, as specified by [P2255R2](https://wg21.link/P2255).

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Added tests for throwing copy/move constructors and assignments. (#189)
- Added tests for the `swap()` exception-safety fix.
- Added tests for the P2255R2 dangling-reference rejection, including the builtin trait path.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
