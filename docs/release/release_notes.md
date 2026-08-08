**🔧 Fixed Bugs**
- `operator*` now returns a `noexcept` function pointer when the stored signature includes `noexcept`, instead of returning `nullptr`.

**⚠️ Breaking Changes**
- `ebd::fn` and `ebd::unique_fn` now call `std::terminate()` on empty call instead of throwing `std::bad_function_call`. If you need `std::bad_function_call` on empty call, use the new `ebd::classic_fn`.
- `make_fn` now preserves the `noexcept` qualifier when deducing signatures for lambdas and functors (it was previously stripped for `ebd::fn`/`ebd::unique_fn`). For example, `make_fn([]() noexcept {})` now yields `fn<void() const noexcept>` instead of `fn<void() const>` (Since C++17).
- `ebd::safe_fn` is deprecated. Use `ebd::fn` for terminate-on-empty, or `ebd::basic_fn` directly if you need the `AssertObjectNoThrow` guarantee.
- The macro `EMBED_FN_CONFIG_USE_BIG_DEFAULT_BUFFER` has been removed.
- If the user creates the `fn_ref` object from `nullptr` in the **debugging mode**, then regardless of whether the user has defined the macro `EMBED_FN_HOOK_DEBUG` or not, the `std::terminate()` function will be called.

**✨ New Features**
- Added `ebd::classic_fn`, a copyable wrapper that throws `std::bad_function_call` on empty call (like `std::function`).
- Added a `make_fn` overload for `noexcept` function pointers (C++17+), which preserves the `noexcept` qualifier in the wrapper's signature.
- noexcept qualifiers are now propagated through `make_fn` for member function pointers and member object pointers.
- `make_fn<FnWrapper>` now accepts multiple arguments, enabling in-place construction with a specific wrapper type, e.g. `make_fn<ebd::classic_fn>(std::in_place_type<Functor>, args...)`.

**🛠️ Optimizations and Improvements**
- Refactored noexcept propagation: replaced the complex `noexcept_qualify_like` trait with a simpler `get_correct_signature` trait that decides based on wrapper config (`IsView || !IsThrowing`).
- Removed `sig_with_noexcept` from `get_unique_signature`; `type` now directly includes `noexcept` when applicable.
- Removed outdated `@todo` and `@experimental` comments.

**📌 Notes**
- None.