# `ebd::fn_view`

## Overview

`ebd::fn_view` is a function object view for callable objects. It is an alias of `ebd::detail::function` with specific configuration parameters optimized for non-owning views, similar to `std::function_ref`.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Signature` | Function signature, e.g., `Ret(Args...)` or `Ret(Args...) const`. |
| `Unused` | Unused parameter. Included for consistency with other function wrapper templates. |

## Configuration

`ebd::fn_view` is configured with the following parameters:

| Configuration | Value | Description |
|---------------|-------|-------------|
| `IsCopyable` | `true` | The view is copyable (but it doesn't copy the underlying callable). |
| `IsView` | `true` | This is a non-owning view of the callable object. |
| `IsThrowing` | `false` | The wrapper will not throw `std::bad_function_call` when called in an empty state; instead, it will call `std::terminate()`. |
| `AssertObjectNoThrow` | `false` | The callable object does not need to be nothrow-constructible or nothrow-destructible. |

## Member Functions

All member functions of `ebd::detail::function` are available for `ebd::fn_view`. For detailed documentation, see [`ebd::detail::function`](./detail/function.md).

## Usage Examples

### Basic Usage

```cpp
#include "embed/embed_function.hpp"

// Create a function view for a void(int) signature

// From a function pointer
void foo(int x) { /* do something */ }
ebd::fn_view<void(int)> view = &foo;
view(42);

// From a lambda
auto lambda = [](int x) { /* do something */ };
ebd::fn_view<void(int)> view2 = lambda;
view2(42);

// From a functor
struct Functor {
    void operator()(int x) { /* do something */ }
};
Functor f;
ebd::fn_view<void(int)> view3 = f;
view3(42);
```

### As Function Parameter

```cpp
// Pass a callable as a view to a function
void process_data(int value, ebd::fn_view<void(int)> callback) {
    // Do some processing
    callback(value);
}

// Usage
process_data(42, [](int x) { /* handle result */ });

// Or with a function pointer
void handle_result(int x) { /* handle result */ }
process_data(100, &handle_result);
```

## Notes

- `ebd::fn_view` is a non-owning view, so the underlying callable object must outlive the view.
- `ebd::fn_view` is trivially copyable and has minimal overhead.
- The buffer size is fixed to `detail::default_buffer_size::view_buf`, which is sufficient to store function pointers and member pointers.
- When called in an empty state, `ebd::fn_view` will call `std::terminate()` instead of throwing an exception.
- `ebd::fn_view` cannot be initialized with rvalue references, as it would create a dangling reference.

## See Also

- [`ebd::detail::function`](./detail/function.md) - The underlying implementation
- [`ebd::fn`](./fn.md) - For copyable callables
- [`ebd::unique_fn`](./unique_fn.md) - For move-only callables
- [`ebd::safe_fn`](./safe_fn.md) - For exception-safe callables