# `ebd::safe_fn`

## *Deprecated*

Please use [`ebd::fn`](./fn.md) instead.

### Migration Note

The `safe_fn` alias still works but has been deprecated. If you migrate to `ebd::fn`, note that `AssertObjectNoThrow` changes to `false`, which means `ebd::fn` no longer requires the callable object's copy constructor or its move constructor, when present, to be `noexcept`; a deleted move constructor is allowed. Its destructor must still be `noexcept`, since `ebd::fn` owns (and therefore destroys) the target. If you need the `AssertObjectNoThrow=true` guarantee, use `ebd::basic_fn` directly:

```cpp
template <
    typename Signature,
    std::size_t BufferSize = ebd::detail::default_values::owning::buffer_size,
    std::size_t Alignment = ebd::detail::default_values::owning::alignment
>
using my_safe_fn = ebd::basic_fn<
    Signature, BufferSize, Alignment,
    true,  // IsCopyable
    false, // IsView
    false, // IsThrowing
    true   // AssertObjectNoThrow
>;
```
