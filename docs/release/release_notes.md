## New

- Added automated release workflow for packaging `header_only.zip` and `header_only.tar.gz`.

- Made `ebd::fn_view` [trivially copyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable.html) across all compilers.

- Added "Future learning & evolution reference" section to README.md.

- `ebd::fn_view::operator()` is now unconditionally `const`-qualified.

## Fix

- Fixed `ebd::fn_view` to support construction from non-movable callable objects.

## Notes

The proposal to reduce the size of `ebd::fn_view` to `2 * sizeof(void(*)())` in order to pass parameters through registers under most calling conventions is not adopted in the current version.

This is because it will cause the packaging logic of member function pointers (PMF) to become counterintuitive, and consequently make the packaging of function pointers equally unpredictable.

As pointed out in the early discussion result of `std::function_ref` [P0792](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0792r5.html#polls): Ensuring that the size is less than or equal to `2 * sizeof(void(*)())` is not mandatory.

If you want to try and test the fn_view of size `2 * sizeof(void(*)())`, you can use the following custom type:

```cpp
template <typename Signature, std::size_t Unused = 0>
using tiny_fn_view = ebd::basic_fn<Signature, sizeof(void(*)()), true, true, false, false>;
```

This proposal is shelved pending further discussion.
