# x86_64 GCC Assembly Analysis

## Free function & scalar parameters

### Source code

```cpp
#include "embed/embed_function.hpp"

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

auto main() -> int {
    auto ebd_add = ebd::make_fn<ebd::classic_fn>(add);
    auto ebd_sub = ebd::make_fn<ebd::classic_fn>(sub);

    auto std_add = std::function<int(int, int)>{ add };
    auto std_sub = std::function<int(int, int)>{ sub };

    volatile int res = 0;

    res = ebd_add(0x2233, 0x1122);
    res = ebd_sub(0x2233, 0x1122);
    res = std_add(0x2233, 0x1122);
    res = std_sub(0x2233, 0x1122);
}
```

### Assembly analysis

```bash
g++ -std=c++11 -Os     # x86-64 gcc (trunk) 17.0.0, System V ABI
```

Both wrappers are 32 bytes on the stack and both dispatch through two levels: a per-type invoker stored inside the object, and then the callable itself. The per-call argument path and the code size per call site are what differ.

### `ebd::classic_fn`: arguments stay in registers

```asm
        mov     edx, 4386                  ; arg#2 -> register
        mov     esi, 8755                  ; arg#1 -> register
        lea     rdi, [rsp+64]              ; the erased object
        ; There is no judgment for detecting the empty state.
        call    [QWORD PTR [rsp+88]]       ; m_invoker

; second level: InvokerImpl<...>::inplace::invoke<int (*)(int, int)>(ErasurePass, int, int)
        mov     rax, rdi
        mov     edi, esi                   ; arg#1 forwarded in a register
        mov     esi, edx                   ; arg#2 forwarded in a register
        mov     rax, QWORD PTR [rax]
        jmp     rax                        ; tail call to `add`/`sub`
```

19 bytes for the call shown. The first of the two `ebd` calls is emitted as a direct call to the very same invoker; the second goes through memory.

`smart_forward_t` turns register-passable parameters into by-value parameters, so the invoker is typed `Ret (*)(ErasurePass, int, int)` instead of `Ret (*)(ErasurePass, int&&, int&&)`. No scalar argument touches memory.

### `std::function`: arguments are spilled to memory

```asm
        mov     DWORD PTR [rsp+24], 8755   ; arg#1 -> stack
        mov     DWORD PTR [rsp+28], 4386   ; arg#2 -> stack
        cmp     QWORD PTR [rsp+144], 0     ; _M_empty()
        jne     .L28
        call    "std::__throw_bad_function_call()"
.L28:
        lea     rdx, [rsp+28]              ; &arg#2
        lea     rsi, [rsp+24]              ; &arg#1
        lea     rdi, [rsp+128]             ; &_M_functor
        call    [QWORD PTR [rsp+152]]      ; _M_invoker

; second level: _Function_handler<...>::_M_invoke(_Any_data const&, int&&, int&&)
        mov     rax, QWORD PTR [rdi]
        mov     edi, DWORD PTR [rsi]       ; arg#1 loaded back from the stack
        mov     esi, DWORD PTR [rdx]       ; arg#2 loaded back from the stack
        jmp     rax
```

43 bytes per call site, 48 bytes including the out-of-line `__throw_bad_function_call` that GCC places between the branch and its target.

`std::function<R(Args...)>::operator()` takes `Args...` by value and forwards them as `Args&&`, so every scalar argument is materialized in memory and passed by address, only to be loaded again by the callee.

### Summary

| | `ebd::classic_fn` | `std::function` |
|---|---|---|
| dispatch levels per invocation | 2 | 2 |
| scalar arguments | registers | stored to stack, address passed, loaded back |
| empty state | `m_invoker` points to `empty::invoke` | `_M_empty()` test and branch on every call |
| call site size | 19 bytes | 43 bytes (48 with the throw call) |
| `sizeof` / `alignof` | 32 / 16 | 32 / 8 |

No empty-state test appears in the generated code for `ebd`: the empty constructor stores `empty::invoke` as `m_invoker`, and calling it throws. `std::function` re-tests `_M_manager` on every invocation.

### Destruction and copy

```asm
; ebd: one dispatch through the manager vtable. Its third entry is NULL for a
; trivially destructible callable, so no indirect call is made.
"ebd::...::destructor_impl<...>::~destructor_impl()":
        mov     rax, QWORD PTR [rdi+16]
        mov     rax, QWORD PTR [rax+16]
        test    rax, rax
        je      .L24
        jmp     rax
.L24:
        ret

; std::function: _M_manager is always invoked with __destroy_functor
"std::_Function_base::~_Function_base()":
        mov     rax, QWORD PTR [rdi+16]
        test    rax, rax
        je      .L21
        mov     rsi, rdi
        push    rdx
        mov     edx, 3
        call    rax
        pop     rcx
.L21:
        ret
```

The manager vtable installed for a function pointer is (names elided):

```asm
        .quad   "trivially_clone<int (*)(int, int)>"   ; 8-byte copy of the payload
        .quad   "trivially_move<int (*)(int, int)>"
        .quad   0                                      ; nothing to destroy
```
