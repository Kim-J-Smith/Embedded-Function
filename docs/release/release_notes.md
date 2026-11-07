**🔧 Fixed Bugs**
- Fixed undefined behavior in `operator*` for non-owning function wrappers (`fn_ref`). (#264)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Owning function wrappers now correctly detect the empty state of a `std::constant_wrapper` that holds a null pointer. (#261)
- Added the `EMBEDDED_FUNCTION_VERSION` macro, encoded as `XYYYZZZ` (for example, `2.5.3` is `2005003L`), for compile-time version checks. (#266)

**🛠️ Optimizations and Improvements**
- Made the GCC 5.4 CI Docker image more robust when downloading CMake. (#257)
- `detail::is_invocable_r` now follows `INVOKE<R>` semantics for array and function return types. Internal trait, no user-visible API change. (#258)
- Added `Clang 20` / `Clang 21` / `Clang 22` CI coverage. (#260)
- Added a workaround for MSVC 19.44 ~ 19.51 so that `volatile`-qualified callables with a static `operator()` work as expected. (#261)
- Best-effort improvements were made for TI 20.2.0.LTS compiler compatibility. (#262)
- CI test scripts now allow skipping the environment setup step and configuring the build directory and parallel job count. (#263)
- Renamed some internal macros to enhance readability. (#265)
- Fixed typos and adjusted assertion message styles. No functional change. (#267)

**📌 Notes**
- None.
