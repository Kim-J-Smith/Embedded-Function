# `ebd::fn_ref`

## Overview

The `ebd::fn_ref` class template is a non-owning call wrapper, that refers to a bound object. Using fn_ref outside of the lifetime of the bound object has *undefined behavior*. It supports const-qualification, volatile-qualification and no-throw guarantees. The qualifications and exception-specification of the signature are respected when invoking the reference function.

It is an alias of `ebd::detail::function` with specific configuration parameters optimized for non-owning views, similar to `std::function_ref`.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `Signature` | Function signature, e.g., `Ret(Args...)` or `Ret(Args...) const`. |

## Configuration

`ebd::fn_ref` is configured with the following parameters:

| Configuration | Value | Description |
|---------------|-------|-------------|
| `IsCopyable` | `true` | The view is copyable (but it doesn't copy the underlying callable). |
| `IsView` | `true` | This is a non-owning view of the callable object. |
| `IsThrowing` | `false` | `ebd::fn_ref` has removed empty state. |
| `AssertObjectNoThrow` | `false` | The callable object does not need to be nothrow-constructible or nothrow-destructible. |

## Member Functions

All member functions of `ebd::detail::function` are available for `ebd::fn_ref`, except:

```cpp
function() = delete;

function(std::nullptr_t) = delete;

template <typename Fn, typename... CArgs>
explicit function(std::in_place_type_t<Fn>, CArgs&&... args) = delete;

template <typename Fn, typename U, typename... CArgs>
explicit function(std::in_place_type_t<Fn>, std::initializer_list<U> il, CArgs&&... args) = delete;

function& operator=(std::nullptr_t) = delete;

constexpr bool is_empty() = delete;

constexpr explicit operator bool() = delete;

void clear() = delete;
```

A view wrapper also has no separate buffer-converting move constructor: an rvalue source binds to the buffer-converting copy constructor, which copies the reference.

For detailed documentation, see [`ebd::detail::function`](./detail/function.md).

## Usage Examples

### Basic Usage

```cpp
#include "embed/embed_function.hpp"

// Create a function view for a void(int) signature

// From a function pointer
void foo(int x) { /* do something */ }
ebd::fn_ref<void(int)> f_ref1 = &foo;
f_ref1(42);

// From a lambda
auto lambda = [](int x) { /* do something */ };
ebd::fn_ref<void(int)> f_ref2 = lambda;
f_ref2(42);

// From a functor
struct Functor {
    void operator()(int x) { /* do something */ }
};
Functor f;
ebd::fn_ref<void(int)> f_ref3 = f;
f_ref3(42);
```

### As Function Parameter

```cpp
// Pass a callable as a view to a function
void process_data(int value, ebd::fn_ref<void(int)> callback) {
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

- `ebd::fn_ref` is a non-owning view, so the underlying callable object must outlive the view.
- `ebd::fn_ref` is trivially copyable and has minimal overhead.
- The buffer size is fixed to `detail::default_values::non_owning::buffer_size`, which is sufficient to store function pointers.
- `ebd::fn_ref` now can be initialized with rvalue references, although it may create a dangling reference. (same as `std::function_ref`)
- The `std::constant_wrapper` (C++26+) in-place constructors (`std::constant_wrapper` + `std::in_place_type_t`) are only available for owning wrappers; `ebd::fn_ref` can only bind an existing object by reference.
- When an `ebd::fn_ref` is created by `ebd::make_fn` from a `std::cw<NTTP-Callable>` plus an object and the first parameter of `NTTP-Callable` is not a reference type, the deduced signature is `const`-qualified, e.g. `ebd::make_fn<ebd::fn_ref>(std::cw<&free_func_add_ii>, a)` yields `ebd::fn_ref<int(int) const>`. Ref-qualifiers transferred from a reference first parameter are dropped, since `ebd::fn_ref` signatures cannot be ref-qualified.

## Compare `ebd::fn_ref` with `std::function_ref`

| Property | `ebd::fn_ref` | `std::function_ref` | Note
| --- | --- | --- | ---
| Object size | `2 * sizeof(void(*)())` | usually `2 * sizeof(void(*)())` | Same
| Trivially copyable | Yes | Yes | Same
| Removed empty state | Yes | Yes | Same
| Support function pointer(FP) | Yes | Yes | Same
| Make `operator()` unconditionally const | Yes | Yes | Same
| Require the target entity to be *Lvalue-Callable* | Yes | Yes | Same
| Do not provide `target()` | Yes | Yes | Same
| Do not provide `target_type()` | Yes | Yes | Same
| Do not provide `operator bool` | Yes | Yes | Same
| Do not provide default constructor | Yes | Yes | Same
| Do not provide comparison with `nullptr` | Yes | Yes | Same
| Constructor is constexpr except from FP | Yes(Since C++20) | Yes | Same
| Can be constructed from `std::constant_wrapper` | Yes(Since C++26) | Yes | Same

## See Also

- [`ebd::detail::function`](./detail/function.md) - The underlying implementation
- [`ebd::fn`](./fn.md) - For copyable callables
- [`ebd::unique_fn`](./unique_fn.md) - For move-only callables
- [`ebd::classic_fn`](./classic_fn.md) - For callables with `std::bad_function_call` on empty
