**🔧 Fixed Bugs**
- Fixed a double-free bug by reordering base classes of `lifetime_operations_impl`. (#189)
- Fixed a bug where treating non-trivial functor as *stateless*. (#190)
- Fixed a double-free bug in `swap()` when a move constructor throws. (#191)
- Fixed undefined behavior caused by reading inactive members of the `ErasurePass` union. (#192)
- Fixed `is_invocable_r` to reject callables that would bind a temporary to a reference return type, as specified by [P2255R2](https://wg21.link/P2255). (#194)
- Fixed a compile error when adapting a callable with a static `operator()` returning non-`void` to a function wrapper whose return type is `void` (C++23 static call operator path). (#194)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Added tests for throwing copy/move constructors and assignments. (#189)
- Added tests for the `swap()` exception-safety fix. (#191)
- Added tests for the P2255R2 dangling-reference rejection, including the builtin trait path. (#194)
- Added tests for adapting a non-`void` static call operator to a `void`-returning function wrapper. (#194)
- Optimized some assertion messages. (#194)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
