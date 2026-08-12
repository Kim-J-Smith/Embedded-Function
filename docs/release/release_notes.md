**🔧 Fixed Bugs**
- Fixed freestanding support: `throw_or_terminate()` now uses `std::terminate()` and the `std::function` overload of `not_empty()` is skipped when `__STDC_HOSTED__` is not defined.
- Fixed GCC 16 ipa-cp devirtualization failure by adding `gcc_ipa_cp_friendly_cast()` so indirect calls via `m_invoker` are tracked correctly (see issue #133).

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Constructed `ErasurePass` directly with const pointers, adding dedicated constructors for const/volatile-qualified erased pointers.
- Simplified `operator()` by inlining the invoker call and dropping the `invoke()` helper from the command tables and removing unnecessary `const_cast`.

**📌 Notes**
- None.