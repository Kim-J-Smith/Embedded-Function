## Fixed

- Fixed a bug where `make_fn` failed to correctly deduce a `noexcept` functor.

- Fixed a bug in converting from `fn_ref` to `fn` / `unique_fn` / `safe_fn`.

## Changed

- **Lowered MSVC requirement to 19.20+** (previously required 19.34+).

- Improved ambiguity error diagnostics for `make_fn` when the callable is ambiguous.

- Enhanced the performance of `fn_ref`, `fn`, `unique_fn`, and `safe_fn`.

- `make_fn` now supports automatic deduction of callable objects with a `static operator()`.

## Notes

- `std::constant_wrapper` support is experimental as of v2.1.4.
