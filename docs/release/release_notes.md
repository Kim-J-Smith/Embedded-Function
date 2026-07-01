## Fixed

- Fixed a double‑free bug in copy and move assignment operators for `ebd::fn`, `ebd::unique_fn`, and `ebd::safe_fn` (#75, #76).
- Added a dedicated lifetime test suite to prevent regressions.

## Changed

- Updated experimental `std::constant_wrapper` support to conform to P4206R0, which reverts the string support introduced in earlier drafts (#68).
- Improved static assertion messages for `make_fn()` and function signatures – they now provide more actionable hints when template deduction fails (#69).
- Relaxed internal manager constraints: `std::is_trivially_copyable` is now used instead of a more restrictive "traditional trivial" check, allowing a wider set of functors to be stored in‑place (#70).
- Refactored the type‑erasure invoker to remove unnecessary cv‑qualifiers and `const_cast` calls, simplifying the codebase without changing public behavior (#71).
- Cleaned up macro formatting and added `!defined(__clang__)` guards for MSVC‑specific code paths.

## Added

- Added a comprehensive set of runtime benchmarks covering creation, copy/move assignment, and various parameter types for `ebd::fn`, `std::function`, `fu2::function`, and `std::move_only_function` (#73, #77).
- Introduced nullability annotations (`_Nonnull`/`_Notnull_`) on the `fn_ref` constructor from function pointers to aid static analysis (#69).
- Enabled Windows CI for the benchmark suite (#73).

## Notes

- `std::constant_wrapper` support is experimental as of v2.1.7.
- The experimental `constant_wrapper` support has been updated to align with P4206R0, removing the previously available string support. Users who were experimenting with that feature may need to adjust their code.