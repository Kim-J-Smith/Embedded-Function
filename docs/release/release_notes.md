## Added

- Specialized assignment operators for `fn_ref` to eliminate unnecessary
  `swap()` overhead.

## Fixed

- Corrected template argument deduction in `fn_ref` constructors accepting
  `std::constant_wrapper`.

## Changed

- Renamed hook macro `EMBED_FN_HOOK_TRACE_EMPTY_CALL` to `EMBED_FN_HOOK_DEBUG`.

## Notes

- `std::constant_wrapper` support is experimental as of v2.1.2.
