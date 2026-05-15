## Fixed

- Fixed bug in deducing `noexcept` functor when using `make_fn`.

- Fixed bug in converting from `fn_ref` to `fn` / `unique_fn` / `safe_fn`.

## Changed

- **Embedded-Function now supports MSVC 19.20+ (previously required MSVC 19.34+).**

- Provided better ambiguity error log for `make_fn`.

- Enhanced the performance of `fn_ref` / `fn` / `unique_fn` / `safe_fn`.

## Notes

- `std::constant_wrapper` support is experimental as of v2.1.4.
