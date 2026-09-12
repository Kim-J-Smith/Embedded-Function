**🔧 Fixed Bugs**
- Fixed a bug where `ebd::make_fn(std::cw<...>, obj)` failed to compile when `obj` was move-only; it now returns `ebd::unique_fn`.
- Fixed a bug where `ebd::make_fn(std::cw<...>, obj)` could not deduce the correct `const`-qualifier of the signature.
- Fixed an internal macro hygiene issue where `EMBED_DETAIL_STATIC_CALL_INVOKER_IMPL` was left defined after use; it is now undefined together with the other internal invoker macros.

**⚠️ Breaking Changes**
- `ebd::make_fn(std::cw<NTTP-Callable>, obj)` and `ebd::make_fn<...>(std::cw<NTTP-Callable>, obj)` now deduce a `const`-qualified signature when the first parameter of the `NTTP-Callable` is not a reference type. For example, for a `std::cw` of an `int(*)(int, int)` function, the deduced signature changed from `int(int)` to `int(int) const` because the first parameter `int` is not a reference type. The qualifiers of the object type are preserved only when the first parameter of the callable is a reference type.

**✨ New Features**
- Owning polymorphic function wrappers (`ebd::fn`, `ebd::unique_fn`, `ebd::classic_fn`) can now be constructed from `{std::cw<...>, std::in_place_type<T>, CArgs...}` and `{std::cw<...>, std::in_place_type<T>, {std::initializer_list}, CArgs...}` since C++26, as an **experimental** exploration of [P2511: Beyond operator(): NTTP callables in type-erased call wrappers](https://wg21.link/P2511). The object is constructed in place inside the wrapper buffer and binds to the first parameter of the callable.
- Added `ebd::make_fn(std::cw<...>, std::in_place_type<T>, CArgs...)` and `ebd::make_fn(std::cw<...>, std::in_place_type<T>, std::initializer_list<U>, CArgs...)`, which deduce the wrapper type, the signature, the buffer size and the alignment. Both return `ebd::fn`, or `ebd::unique_fn` when the object is not copy-constructible.

**🛠️ Optimizations and Improvements**
- Unified the template parameter naming and the documentation comments of the `std::constant_wrapper` support, and corrected the `@return` documentation of the `ebd::make_fn` overloads.
- Switched the MSVC CI tests to the Ninja generator (`test/script/test-msvc.bat`), which speeds up the CI test builds.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
- The `std::constant_wrapper` + `std::in_place_type_t` support is still **experimental**.
