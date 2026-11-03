**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Owning function wrappers now correctly detect the empty state of a `std::constant_wrapper` that holds a null pointer.

**🛠️ Optimizations and Improvements**
- Made the GCC 5.4 CI Docker image more robust when downloading CMake.
- `detail::is_invocable_r` now follows `INVOKE<R>` semantics for array and function return types. Internal trait, no user-visible API change.
- Added `Clang 20` / `Clang 21` / `Clang 22` CI coverage. 
- Added a workaround for MSVC 19.44 ~ 19.51 so that `volatile`-qualified callables with a static `operator()` work as expected.

**📌 Notes**
- None.
