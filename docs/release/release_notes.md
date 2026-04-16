## New

- Add a fallback to `make_fn(...)` to avoid overwhelming template error messages.

- Mark the class `ebd::detail::function` as *final*.

## Fix

- Fix a bug in `ErasureCore` to comply with [P0137](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0137r1.html).

- Fix a bug in `make_fn<ebd::fn_view>` when called with a free function.

## Notes

- Since `std::function_ref` has no empty state, setting `ebd::fn_view` to null is not recommended.
