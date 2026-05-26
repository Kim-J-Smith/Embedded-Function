## Fixed

- Fixed a bug where empty and trivial functor could be incorrectly treated as stateless.

- Fixed MSVC 19.10–19.14 compatibility issue.

## Changed

- `fn_ref` now accepts temporary callable objects (previously disallowed rvalue references).

- `basic_fn` no longer automatically aligns `BufferSize`.

## Added

- Added CTAD guides for `fn_ref` with `std::constant_wrapper`.

## Notes

- `std::constant_wrapper` support is experimental as of v2.1.5.
