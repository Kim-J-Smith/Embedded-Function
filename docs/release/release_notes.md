**🔧 Fixed Bugs**
- Fixed a bug where MSVC mistakenly regarded the empty-state invoker (`empty::invoke`, which throws `std::bad_function_call` or terminates) as a hot path and peeled it into a per-call guard in the calling code. The `empty::invoke` is now marked with the new internal `EMBED_DETAIL_COLD` macro to address this issue.
- Fixed a bug where a callable given through `std::constant_wrapper` lost its ref-qualifier in the deduced signature.
- Fixed a bug where `ebd::fn<int(int)>` accepted an `&`-qualified callable. The `std::constant_wrapper` + object constructors now additionally require the callable to be invocable with the object carrying the same qualifiers of the signature.

**⚠️ Breaking Changes**
- `make_fn(std::cw<&T::f>, T{})` now preserves the ref-qualifier of the member function in the deduced signature, so an `&`- or `&&`-qualified `T::f` yields `fn<..., Ret(Args...) const & noexcept>` or `fn<..., Ret(Args...) const && noexcept>` instead of collapsing both to `fn<..., Ret(Args...) const noexcept>`. This is the fix above; it changes the deduced type.
- Callables with a by-value explicit object parameter (`this T self`) now deduce a `const`-qualified signature, e.g. `fn<..., Ret(Args...) const noexcept>` instead of `fn<..., Ret(Args...) noexcept>`.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Updated the assembly analysis documents under `docs/perf/`: the x86_64 MSVC, RISC-V GCC and ARM GCC analyses now compare register argument passing against the stack spills of `std::function` and cover destruction/copy, the `ebd::fn_ref` zero-stack analysis was extended with a full comparison and a summary table, and a new `docs/perf/x86_64_gcc_asm_analysis.md` document was added.
- Renamed some internal traits, tags and the `cxx_traits` namespace to make them more readable.
- More internal functions now use `requires` instead of `enable_if` when compiled as *C++20* or later.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
- The `std::constant_wrapper` + `std::in_place_type_t` support is still **experimental**.
