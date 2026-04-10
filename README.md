# Embedded Function

<p align="center">
  <img src="https://img.shields.io/badge/Version-2.0.11-yellow?style=for-the-badge&logo=github" alt="Version - 2.0.11">
  <img src="https://img.shields.io/badge/License-MIT-orange?style=for-the-badge" alt="License - MIT">
  <img src="https://img.shields.io/badge/C++-11/14/17/20/23-blue?style=for-the-badge&logo=c%2B%2B" alt="C++ - 11/14/17/20/23">
</p>

<p align="center">
  <a href="https://github.com/Kim-J-Smith/Embedded-Function/actions/workflows/test.yml">
    <img src="https://img.shields.io/badge/GCC_C++11/14/17/20/23-passing-brightgreen?style=flat" alt="gcc-C++11/14/17/20/23 - passing">
    <img src="https://img.shields.io/badge/Clang_C++11/14/17/20/23-passing-brightgreen?style=flat" alt="clang-C++11/14/17/20/23 - passing">
    <img src="https://img.shields.io/badge/MSVC_C++14/17/20/23-passing-brightgreen?style=flat" alt="msvc-C++14/17/20/23 - passing">
  </a>
</p>

> *Embedded [std::function](http://en.cppreference.com/w/cpp/utility/functional/function) alternative: lightweight, deterministic, heap-free.*

## 📌 Overview

**Embedded Function** is an embedded-friendly lightweight function wrapper implemented based on the C++11 standard, tailored specifically for embedded systems. 

While functionally and conceptually analogous to *std::function*, it offers substantially reduced overhead and superior real-time performance characteristics. **Notably, Embedded Function eliminates dynamic heap memory allocations entirely**, ensuring deterministic execution behavior and predictable real-time performance for embedded applications.

In only **one** [header file](./include/embed/embed_function.hpp), **4** function wrappers are provided as follows:

```cpp
namespace ebd {
  // Wrapper for copyable callable objects.
  template <class Signature, size_t BufferSize> fn;
  // Wrapper for movable, especially move-only callable objects.
  template <class Signature, size_t BufferSize> unique_fn;
  // Wrapper for copyable callable objects which assert no-throw in Ctor and Dtor.
  template <class Signature, size_t BufferSize> safe_fn;
  // View (aka reference) for callable objects.
  template <class Signature, size_t Unused> fn_view;
}
```

## ⚡ Quick start
- Clone the repository or download the `header_only.zip` in the "Release".

- Add include path `<repo_root>/include`.

- In program `#include "embed/embed_function.hpp"`.

- Use the `ebd::fn` template class.

```cpp
#include "embed/embed_function.hpp"

struct Example {
    static void static_mem_fn(int) {};
    void mem_fn(int) const {};
    void operator()(int) {};
};

auto main() -> int {
    Example e;
    ebd::fn<void(int)> fn_;

    fn_ = &Example::static_mem_fn;
    fn_(123);

    fn_ = [e](int arg) { e.mem_fn(arg); };
    fn_(456);

    fn_ = e;
    fn_(789);
}
```

## 🔧 Wrapper definition syntax

```cpp
/// The definition of method of a function wrapper is as follows:
ebd::fn<int (int, float, char) const, 3*sizeof(void*)> fn_;
//       ^     ^     ^     ^     ^        ^
//       |     |     |     |     |        |
// Return type |     |     |     |        |
// Parameters ~|~~~~~|~~~~~|     |        |
// Qualifier ~~~~~~~~~~~~~~~~~~~~|        |
// Buffer size ~~~~~~~~~~~~~~~~~~~~~~~~~~~|
```

> The *`Qualifier`* is used to restrict the callable objects wrapped within `ebd::fn`, rather than `ebd::fn` itself. In other words, the `operator()` of the `ebd::fn` object will be qualified with the `Qualifier` modifier.

> The *`Buffer size`* is the size used to store the callable object, which can be omitted. If omitted, this parameter will be set to `detail::default_buffer_size::value` by default, which is sufficient to store most common callable objects, including function pointers, simple non-capturing and capturing lambdas, and lightweight custom classes.

## 🧠 Design goals driving the design

  - Should behave close to a normal function pointer. Small, efficient, no heap allocation.

  - Support the packaging of all callable objects in C++, including:
    - Free function.
    - Lambda function.
    - Functor.
    - Static member function.
    - Member function.

  - Be usable with C++11 while offering more functionality for later editions.

  - Be constexpr and exception friendly. As much as possible should be declared constexpr and noexcept.

  - Should be based on the analysis of [N4159](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4159.pdf), [P2548](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2548r6.pdf) and [LWG2393](https://cplusplus.github.io/LWG/issue2393), and should avoid repeating the mistakes made by `std::function`. Therefore, *Embedded Function* should:

    - *NOT* implement the method `target()` and `target_type()`.
    - Allow the application of qualifiers, such as `const`, `volatile`, `&` and `&&`, to the function signature.
    - Ensure that the qualifier of the underlying object is consistent or more restrictive than that of the function signature.

  - Learn and refer to the optimization experience of `std::function` in [libstdc++](https://gcc.gnu.org/cgit/gcc/commit/?id=d38d26be33aba5d4c12429478375a47c474124d2), [libc++](https://reviews.llvm.org/D55045) and [Microsoft C++ Standard Library](https://github.com/microsoft/STL/issues/969).

  - Provide a view or reference to the callable object, referring to the [`std::function_ref` P0792](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p0792r14.html).

  - Following the above design goals, `ebd::fn`, `ebd::unique_fn`, `ebd::safe_fn` and `ebd::fn_view` were designed for developers to use.

## ✨ Core function wrappers

### Summary table

| Wrapper Type | Copyable | View (Non-owning) | Throws on Empty Call | Assert No-Throw (Ctor/Dtor) | Buffer Size | Primary Use Case |
| :----------- | :---: | :---: | :---: | :---: | :---: | :---: |
| `ebd::fn`    |  Yes  |   No  | Yes (`std::bad_function_call`) | No | Configurable (aligned, default: `sizeof(void(Class::*)())`) | Copyable callable wrapper |
| `ebd::unique_fn`    |  No  |   No  | Yes (`std::bad_function_call`) | No | Configurable (aligned, default: `sizeof(void(Class::*)())`) | Move-only callable wrapper |
| `ebd::safe_fn`    |  Yes  |   No  | No (`std::terminate()`) | Yes | Configurable (aligned, default: `sizeof(void(Class::*)())`) | Exception-safe copyable callable wrapper |
| `ebd::fn_view`    |  Yes  |   Yes  | No (`std::terminate()`) | No | Fixed (`sizeof(void(Class::*)())`, template param unused) | Lightweight non-owning view of callables |

### Key takeaways

1. **Ownership & Copy**: `fn`/`safe_fn` own callables (copyable), `unique_fn` owns but is move-only, `fn_view` is non-owning (view).

2. **Exception Behavior**: Only `fn`/`unique_fn` throw on empty calls; `safe_fn`/`fn_view` terminate (no exceptions).

3. **Buffer Configuration**: `fn`/`unique_fn`/`safe_fn` support configurable buffer sizes (aligned), while `fn_view` uses a fixed buffer (unused template param).

4. **Triviality**: In Clang, after version *15.0.0*, `fn_view` is trivially copy-constructible, trivially move-constructible, trivially copy-assignable, trivially move-assignable, and trivially destructible. After Clang *21.1.0*, expressions `std::is_trivially_relocatable_v<ebd::fn_view<...>>`, `__builtin_is_cpp_trivially_relocatable(ebd::fn_view<...>)` and `__is_trivially_relocatable(ebd::fn_view<...>)` are *`true`*.

## 🚀 Performance optimization

### Branch elimination

`ebd::fn` / `ebd::unique_fn` / `ebd::safe_fn` / `ebd::fn_view` completely eliminate runtime checks for empty function states during invocation, significantly boosting performance of frequent function calls.

### Smart forwarding

`ebd::fn` / `ebd::unique_fn` / `ebd::safe_fn` / `ebd::fn_view` enable scalar arguments and small-sized trivial arguments to be passed via registers instead of having to be passed via the stack as in `std::function`. This significantly reduces the memory access overhead during parameter passing.

> Click [x64-asm](./docs/perf/x86_64_msvc_asm_analysis.md), [rv32-asm](./docs/perf/riscv_gcc_asm_analysis.md) and [arm32-asm](./docs/perf/arm_gcc_asm_analysis.md) to see more details.

## 🧩 Automatic deduction

### Brief introduction

In order to simplify the use of `ebd::fn`, function `ebd::make_fn()` is provided, which can automatically deduce the signature and buffer size of the callable object and create a `ebd::fn` or `ebd::unique_fn` object. (Return `ebd::unique_fn` only when the callable object is of the move-only type.)

> __NOTE__: 
> The [Concepts](https://en.cppreference.com/w/cpp/language/constraints.html) language feature is available for use provided that the compiler is configured to support the C++20 standard. On platforms that do not support C++20, `enable_if` will be used instead.

### Usage

- `[]` means optional.
- `Signature`: The signature of the callable object. (such as `void(int)`)
- `BufferSize`: The buffer size of the callable object. (such as `2*sizeof(void*)`)

```cpp
// Create empty ebd::fn with specified signature and buffer size.
// If the BufferSize is omitted, it will be set by default (usually 2*sizeof(void*)).
auto f = ebd::make_fn<Signature[, BufferSize]>();
auto f = ebd::make_fn<Signature[, BufferSize]>(nullptr);
```

```cpp
// Create ebd::fn or ebd::unique_fn from unambiguous callable object.
// If the Signature is omitted, the signature will be deduced from Callable_Object.
auto f = ebd::make_fn[<Signature>](Callable_Object);
```

```cpp
// Create ebd::fn or ebd::unique_fn from ambiguous callable object with specified signature, such as overload free function, overload member function, etc.
auto f = ebd::make_fn<Signature>(Ambiguous_Callable_Object);
```

```cpp
// Create specified function wrapper and automatically deduce the template arguments.
// The Callable_Object should be unambiguously callable (non-overload).
auto f = ebd::make_fn<ebd::fn>(Callable_Object);
auto f = ebd::make_fn<ebd::unique_fn>(Callable_Object);
auto f = ebd::make_fn<ebd::safe_fn>(Callable_Object);
auto f = ebd::make_fn<ebd::fn_view>(Callable_Object);
```

```cpp
// In place build functor within buffer. Functor should be unambiguously callable (non-overload).
auto f = ebd::make_fn(std::in_place_type<Functor>, CArgs...); // Since C++17
auto f = ebd::make_fn(std::in_place_type<Functor>, {/*std::initializer_list*/}, CArgs...); // Since C++17
```

## 🔗 Back to function pointer

### Brief introduction

In embedded MCU development, it is often necessary to pass a C-style free function pointer as an argument, as existing libraries are typically written in C. To address this, we have implemented an `operator*` overload that simplifies converting an object of type `ebd::fn` / `ebd::unique_fn` / `ebd::safe_fn` / `ebd::fn_view` to a C-style free function pointer.

If the object encapsulated by the function wrapper is a valid function pointer, this mechanism returns the pointer; otherwise, it returns nullptr. Basically, it is equivalent to a highly restricted `target()` method.

### Example

```cpp
void free_function() {}
struct Functor { void operator()() {} };

ebd::fn<void()> fn_ = &free_function;
void(*free_function_pointer)() = *fn_;
ASSERT_EQ(free_function_pointer, &free_function);

fn_ = +[]() { /* ... */ }; // lambda -> function pointer
free_function_pointer = *fn_;
ASSERT_NE(free_function_pointer, nullptr); // NOT equal nullptr

fn_ = []() { /* ... */ };
free_function_pointer = *fn_;
ASSERT_EQ(free_function_pointer, nullptr);

fn_ = Functor{};
free_function_pointer = *fn_;
ASSERT_EQ(free_function_pointer, nullptr);
```

## 📦 C++20 Module support

### Brief introduction

**Embedded Function** provides support for C++20 modules. You can wrap the library into a module according to the guide below.

### Usage

To create a module named `ebd.function`, create a module interface file (e.g., `ebd_function.cppm` or `ebd_function.ixx`):

```cpp
module;
#include "embed/embed_function.hpp"
export module ebd.function;

export namespace ebd {
  using ::ebd::fn;
  using ::ebd::unique_fn;
  using ::ebd::safe_fn;
  using ::ebd::fn_view;
  using ::ebd::make_fn; // NOLINT(misc-unused-using-decls)
}
```

Then you can use it in other files:

```cpp
import ebd.function;

auto main() -> int {
    ebd::fn<void()> fn1 = []() { /* ... */ };
    ebd::unique_fn<void()> fn2 = []() { /* ... */ };
    ebd::safe_fn<void()> fn3 = []() { /* ... */ };
    ebd::fn_view<void()> fn4 = fn2;
    auto fn5 = ebd::make_fn([]() { /* ... */ });

    fn1(); fn2(); fn3(); fn4(); fn5();
}
```

## ✅ Compatibility

Every compiler with modern C++11 support should work.
*Embedded Function* only depends on the standard library.

- GCC 5.1+
- Clang 3.7+
- MSVC v19.34+ (VS17.4+)

## 🧪 Test

Go to the `<root>/test/` directory, and follow the instructions in [`HOW-TO-TEST.md`](./test/HOW-TO-TEST.md) to run the tests.

## ⏱️ Benchmark

Go to the `<root>/benchmark/` directory, and follow the instructions in [`HOW-TO-BENCHMARK.md`](./benchmark/HOW-TO-BENCHMARK.md) to run the tests.

> *( Compiler: `MSVC` Standard: `C++14` Config: `Release` Tool: [picobench](https://github.com/iboB/picobench) )* 

> **std**: `std::function`, **ebd**: `ebd::fn`, **fu2**: [`fu2::function`](https://github.com/Naios/function2)

```md
## FreeFunction.ScalarParameters:

 Name (* = baseline)      |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
--------------------------|--------:|----------:|--------:|-------:|----------:
 free_scalar_std *        |   10000 |     0.030 |       3 |      - |332225913.6
 free_scalar_ebd          |   10000 |     0.028 |       2 |  0.930 |357142857.1
 free_scalar_fu2          |   10000 |     0.052 |       5 |  1.731 |191938579.7
 free_scalar_std *        |  100000 |     0.301 |       3 |      - |332667997.3
 free_scalar_ebd          |  100000 |     0.265 |       2 |  0.881 |377643504.5
 free_scalar_fu2          |  100000 |     0.523 |       5 |  1.742 |191021967.5
 free_scalar_std *        | 1000000 |     3.006 |       3 |      - |332712270.4
 free_scalar_ebd          | 1000000 |     2.708 |       2 |  0.901 |369317132.6
 free_scalar_fu2          | 1000000 |     5.264 |       5 |  1.751 |189958778.9

## FreeFunction.TrivialParameters:

 Name (* = baseline)      |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
--------------------------|--------:|----------:|--------:|-------:|----------:
 free_trivial_std *       |   10000 |     0.032 |       3 |      - |311526479.8
 free_trivial_ebd         |   10000 |     0.024 |       2 |  0.754 |413223140.5
 free_trivial_fu2         |   10000 |     0.052 |       5 |  1.626 |191570881.2
 free_trivial_std *       |  100000 |     0.322 |       3 |      - |310366232.2
 free_trivial_ebd         |  100000 |     0.240 |       2 |  0.746 |415800415.8
 free_trivial_fu2         |  100000 |     0.510 |       5 |  1.583 |196001568.0
 free_trivial_std *       | 1000000 |     3.222 |       3 |      - |310375865.2
 free_trivial_ebd         | 1000000 |     2.508 |       2 |  0.778 |398692289.3
 free_trivial_fu2         | 1000000 |     5.792 |       5 |  1.798 |172660876.8

## FreeFunction.CopyHardParameters:

 Name (* = baseline)      |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
--------------------------|--------:|----------:|--------:|-------:|----------:
 free_copyhard_std *      |   10000 |     0.197 |      19 |      - | 50684237.2
 free_copyhard_ebd        |   10000 |     0.198 |      19 |  1.004 | 50505050.5
 free_copyhard_fu2        |   10000 |     0.303 |      30 |  1.537 | 32981530.3
 free_copyhard_std *      |  100000 |     1.976 |      19 |      - | 50604726.5
 free_copyhard_ebd        |  100000 |     1.982 |      19 |  1.003 | 50456632.5
 free_copyhard_fu2        |  100000 |     3.044 |      30 |  1.541 | 32849352.9
 free_copyhard_std *      | 1000000 |    19.898 |      19 |      - | 50256307.2
 free_copyhard_ebd        | 1000000 |    20.052 |      20 |  1.008 | 49870088.4
 free_copyhard_fu2        | 1000000 |    31.358 |      31 |  1.576 | 31889890.6

## FreeFunction.CallTrivialParameters:

 Name (* = baseline)      |   Dim   |  Total ms |  ns/op  |Baseline| Ops/second
--------------------------|--------:|----------:|--------:|-------:|----------:
 free_calltrivial_std *   |   10000 |     0.032 |       3 |      - |311526479.8
 free_calltrivial_ebd     |   10000 |     0.024 |       2 |  0.751 |414937759.3
 free_calltrivial_fu2     |   10000 |     0.056 |       5 |  1.757 |177304964.5
 free_calltrivial_std *   |  100000 |     0.320 |       3 |      - |312597686.8
 free_calltrivial_ebd     |  100000 |     0.257 |       2 |  0.802 |389711613.4
 free_calltrivial_fu2     |  100000 |     0.584 |       5 |  1.827 |171115674.2
 free_calltrivial_std *   | 1000000 |     3.223 |       3 |      - |310269934.8
 free_calltrivial_ebd     | 1000000 |     2.407 |       2 |  0.747 |415506710.4
 free_calltrivial_fu2     | 1000000 |     5.934 |       5 |  1.841 |168517551.1
```

> See [here](https://github.com/Kim-J-Smith/Embedded-Function/actions/workflows/benchmark.yml) for more benchmark results.

## 🧭 Future learning & evolution reference

- [`llvm::function_ref`](https://reviews.llvm.org/D106784?id=361604).

- [C++26-`std::function_ref`-P0792](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p0792r14.html).

- [C++26-`std::copyable_function`-P2548](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2548r6.pdf).

## 📚 Similar implementations

- [std::function](http://en.cppreference.com/w/cpp/utility/functional/function)

- [Naios/function2](https://github.com/Naios/function2)

- [pmed/fixed_size_function](https://github.com/pmed/fixed_size_function)

- [rigtorp/Function](https://github.com/rigtorp/Function)

- [rosbacke/delegate](https://github.com/rosbacke/delegate)

- [winterscar/functional-avr](https://github.com/winterscar/functional-avr)

- [bdiamand/Delegate](https://github.com/bdiamand/Delegate)

- [potswa/cxx_function](https://github.com/potswa/cxx_function)
