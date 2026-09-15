# x86_64 GCC `fn_ref` Zero-stack overhead

### Source code

```cpp
#include "embed/embed_function.hpp"

bool func(ebd::fn_ref<bool(int, int)> f) {
    return f(1, 2);
}

bool func(std::function<bool(int, int)> f) {
    return f(1, 2);
}
```

### Assembly analysis

```bash
g++ -std=c++11 -Os     # x86-64 gcc (trunk) 17.0.0, System V ABI
```

`ebd::fn_ref<bool(int, int)>` is 16 bytes and trivially copyable, so it is passed
in two registers: `rdi` carries the reference storage and `rsi` carries
`m_invoker`. Nothing is constructed in this translation unit, so no invoker,
manager or vtable symbol is emitted here.

### `ebd::fn_ref`: a tail call, zero stack

```asm
"func(ebd::detail::function<8ul, 8ul, ebd::detail::fn_traits::config_package<true, true, false, false>, bool (int, int)>)":
        mov     rax, rsi      ; m_invoker
        mov     edx, 2        ; arg#2
        mov     esi, 1        ; arg#1
        jmp     rax           ; tail call; rdi already holds the erased callable
```

`rdi` is the invoker's own first parameter, so it needs no shuffling: GCC moves
the invoker out of `rsi` into `rax`, puts the two immediates in `esi` and `edx`,
and the call becomes a `jmp`. Four instructions, no stack frame, no empty-state
test, no `ret`.

### `std::function`: a stack frame per call

```asm
"func(std::function<bool (int, int)>)":
        sub     rsp, 24                            ; frame
        mov     DWORD PTR [rsp+8], 1               ; arg#1 -> stack
        mov     DWORD PTR [rsp+12], 2              ; arg#2 -> stack
        cmp     QWORD PTR [rdi+16], 0              ; _M_empty()
        jne     .L3
        call    "std::__throw_bad_function_call()"
.L3:
        lea     rdx, [rsp+12]                      ; &arg#2
        lea     rsi, [rsp+8]                       ; &arg#1
        call    [QWORD PTR [rdi+24]]               ; _M_invoker
        add     rsp, 24
        ret
```

`std::function` is not trivially copyable, so it is passed by invisible reference
in `rdi`; every call allocates a 24-byte frame, spills both scalar arguments,
passes their addresses, tests `_M_manager` for the empty state, and ends with a
`ret` of its own. Ten instructions against four.

### Summary

| | `ebd::fn_ref` | `std::function` |
|---|---|---|
| stack used by the call | 0 | 24 bytes |
| instructions | 4, ending in `jmp` | 10, with frame and `ret` |
| scalar arguments | registers | spilled to stack, address passed |
| empty state | none | `_M_empty()` test and branch on every call |
| passed in | `rdi` + `rsi` | invisible reference (`rdi`) |
| `sizeof` / `alignof` | 16 / 8 | 32 / 8 |

The zero-stack property is a calling-convention coincidence: in view mode the
erased callable *is* the first register of the wrapper and is also the invoker's
first parameter, so it is already in place. An owning wrapper passes a pointer to
its buffer instead, which has to be materialized with an instruction of its own.
