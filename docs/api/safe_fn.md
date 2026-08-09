# `ebd::safe_fn`

## *Deprecated*

Please use [`ebd::fn`](./fn.md) instead.

### Migration Note

The `safe_fn` alias still works but has been deprecated. If you migrate to `ebd::fn`, note that `AssertObjectNoThrow` changes to `false` — meaning `ebd::fn` no longer requires the callable object to be noexcept-constructible/destructible. If you need the `AssertObjectNoThrow=true` guarantee, use `ebd::basic_fn` directly:

```cpp
template <typename Signature, std::size_t BufferSize = ebd::detail::default_buffer_size::value>
using my_safe_fn = ebd::basic_fn<
    Signature, ebd::detail::get_aligned_size(BufferSize),
    true,  // IsCopyable
    false, // IsView
    false, // IsThrowing
    true   // AssertObjectNoThrow
>;
```
