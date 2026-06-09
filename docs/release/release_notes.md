## Fixed

- No swap in copy assignment.

## Changed

- Rename `is_trivial_for_call` as `is_itanium_trivial_for_calls` and `is_reg_passable` as `is_register_passable`.
- Refactor `is_callable_from` and `is_invocable_using`.
- Add specialization for `is_register_passable` for Windows x64.
- Update README.md and make_fn.md.

## Added

- Add static call operator test.
- Add stateless assign test.

## Notes

- `std::constant_wrapper` support is experimental as of v2.1.6.
