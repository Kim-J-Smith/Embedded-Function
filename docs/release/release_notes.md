## New

- Provide a stable `basic_fn` for user customization.
- Support `std::constant_wrapper` (C++26) for `fn_ref`.
- Make `make_fn()` automatically deduce the `noexcept` specification of the signature for `fn_ref` from the function pointer. (Since C++17)

## Fix

- Fix a bug in the constructor of `fn_ref`.

## Notes

- The support of `std::constant_wrapper` is experimental in v2.1.1.