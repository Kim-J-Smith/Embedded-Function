**🔧 Fixed Bugs**
- `operator*` now returns a `noexcept` function pointer when the stored signature includes `noexcept`, instead of returning `nullptr`. (#117)
- `make_fn(wrapper)` no longer matches the lambda/functor overload; it now correctly takes the copy/move path, so the deduced `BufferSize` is not enlarged unnecessarily. (#127)
- `make_fn<FnWrapper>(wrapper)` now computes the correct `BufferSize`: it uses the deduced size when the wrapper configs are directly convertible, otherwise it sizes the buffer to fit the wrapper object for indirect wrapping. (#127)

**⚠️ Breaking Changes**
- `ebd::fn` and `ebd::unique_fn` now call `std::terminate()` on empty call instead of throwing `std::bad_function_call`. If you need `std::bad_function_call` on empty call, use the new `ebd::classic_fn`. (#117)
- `make_fn` now preserves the `noexcept` qualifier when deducing signatures for lambdas and functors (it was previously stripped for `ebd::fn`/`ebd::unique_fn`). For example, `make_fn([]() noexcept {})` now yields `fn<void() const noexcept>` instead of `fn<void() const>` (Since C++17). (#117)
- `ebd::safe_fn` is deprecated. Use `ebd::fn` for terminate-on-empty, or `ebd::basic_fn` directly if you need the `AssertObjectNoThrow` guarantee. (#117)
- The macro `EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER` has been removed. (#116)
- If the user creates the `fn_ref` object from `nullptr` in the *debugging mode*, then regardless of whether the user has defined the macro `EMBED_FN_HOOK_DEBUG` or not, the `std::terminate()` function will be called. (#121)
- The `ebd::fn_ref` CTAD deduction guides for `std::constant_wrapper` (C++26) have been removed. Use `ebd::make_fn(std::cw<...>)` instead. (#124)

**✨ New Features**
- Added `ebd::classic_fn`, a copyable wrapper that throws `std::bad_function_call` on empty call (like `std::function`). (#117)
- Added a `make_fn` overload for `noexcept` function pointers (C++17+), which preserves the `noexcept` qualifier in the wrapper's signature. (#117)
- noexcept qualifiers are now propagated through `make_fn` for member function pointers and member object pointers. (#117)
- `make_fn<FnWrapper>` now accepts multiple arguments, enabling in-place construction with a specific wrapper type, e.g. `make_fn<ebd::classic_fn>(std::in_place_type<Functor>, args...)`. (#123)
- Added `make_fn` overloads for `std::constant_wrapper` (C++26+), which deduce and return an `ebd::fn_ref` from `std::cw<fn>` or `std::cw<member_fn>` + object. (#124)

**🛠️ Optimizations and Improvements**
- Refactored noexcept propagation: replaced the complex `noexcept_qualify_like` trait with a simpler `get_correct_signature` trait that decides based on wrapper config (`IsView || !IsThrowing`). (#120)
- Removed `sig_with_noexcept` from `get_unique_signature`; `type` now directly includes `noexcept` when applicable. (#117)
- Removed outdated `@todo` and `@experimental` comments. (#116)

**📌 Notes**
- v2.1.11 planned to remove `ebd::fn_view` and `ebd::safe_fn` in v2.2.0. Instead of removal, both are kept as *deprecated* aliases (see the Breaking Changes section above) to avoid breaking existing code.