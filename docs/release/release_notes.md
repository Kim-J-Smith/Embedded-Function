## Fixed

- Fixed a constructor bug in `fn_ref`.

## Changed

- `fn`, `unique_fn`, and `safe_fn` now use `is-callable-from` as the constraint, aligning with [func.wrap.move.ctor]/1. This tightens overload resolution and avoids incorrect constructor participation.

- Added negative compilation tests for the above constraints.

## Notes

- `std::constant_wrapper` support is experimental as of v2.1.3.
