# ARM GCC Assembly Analysis

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
arm-none-eabi-g++ -Os -std=c++11    # GNU Tools for STM32 14.3.rel1 (GCC 14.3.1)
```

The default target is `-mcpu=arm7tdmi -march=armv4t` in ARM state. Without the
register form of `blx` (ARMv5T and later) an indirect call costs two instructions
(`mov lr, pc` + `bx`); on `-mcpu=cortex-m4` the same source emits a single
`blx r3`. `make_fn<ebd::classic_fn>(add)` deduces an `ebd::classic_fn`
(`detail::function<8u, 8u, ...>`): a function pointer needs 4 bytes but the
owning alignment is 8, so the wrapper is 16 bytes, exactly like the
`std::function` used here. Both dispatch through two levels: a per-type invoker
stored inside the object, and then the callable itself.

### `ebd::classic_fn`: arguments stay in registers

```asm
; the second call, which goes through memory
    82dc:	e59f2100 	ldr	r2, [pc, #256]	@ 83e4 <main+0x17c>   ; arg#2
    82e0:	e59f10f8 	ldr	r1, [pc, #248]	@ 83e0 <main+0x178>   ; arg#1
    82e4:	e59d3034 	ldr	r3, [sp, #52]	@ 0x34                 ; m_invoker
    82e8:	e28d0028 	add	r0, sp, #40	@ 0x28                 ; the erased object
    82ec:	e1a0e00f 	mov	lr, pc
    82f0:	e12fff13 	bx	r3

; second level: InvokerImpl<8u, 8u, ...>::inplace::invoke<int (*)(int, int)>(ErasurePass, int, int)
    85c0:	e1a03000 	mov	r3, r0
    85c4:	e1a00001 	mov	r0, r1                                ; arg#1 stays in a register
    85c8:	e5933000 	ldr	r3, [r3]
    85cc:	e1a01002 	mov	r1, r2                                ; arg#2 stays in a register
    85d0:	e12fff13 	bx	r3                                    ; tail call to `add`/`sub`
```

6 instructions, 24 bytes; the first `ebd_add()` call is a direct `bl` instead.

### `std::function`: arguments are spilled to memory

```asm
; the first call (GCC scheduled the `res =` store of the previous statement
; into this sequence; it is not part of the call site)
    82f4:	e59f30e4 	ldr	r3, [pc, #228]	@ 83e0 <main+0x178>   ; arg#1
    82f8:	e58d3008 	str	r3, [sp, #8]                           ; arg#1 -> stack
    82fc:	e59f30e0 	ldr	r3, [pc, #224]	@ 83e4 <main+0x17c>   ; arg#2
    8300:	e58d300c 	str	r3, [sp, #12]                          ; arg#2 -> stack
    8304:	e59d3040 	ldr	r3, [sp, #64]	@ 0x40                 ; _M_manager
    8308:	e3530000 	cmp	r3, #0                                 ; _M_empty()
    830c:	e58d0004 	str	r0, [sp, #4]                           ; previous statement
    8310:	1a000009 	bne	833c <main+0xd4>
    8314:	ebffff43 	bl	8028 <std::__throw_bad_function_call()>
    833c:	e59d3044 	ldr	r3, [sp, #68]	@ 0x44                 ; _M_invoker
    8340:	e28d200c 	add	r2, sp, #12                            ; &arg#2
    8344:	e28d1008 	add	r1, sp, #8                             ; &arg#1
    8348:	e28d0038 	add	r0, sp, #56	@ 0x38                  ; &_M_functor
    834c:	e1a0e00f 	mov	lr, pc
    8350:	e12fff13 	bx	r3

; second level: _Function_handler<...>::_M_invoke(_Any_data const&, int&&, int&&)
    860c:	e1a0c001 	mov	ip, r1
    8610:	e5903000 	ldr	r3, [r0]
    8614:	e5921000 	ldr	r1, [r2]                               ; arg#2 loaded back
    8618:	e59c0000 	ldr	r0, [ip]                               ; arg#1 loaded back
    861c:	e12fff13 	bx	r3
```

13 instructions, 52 bytes on the hot path, plus a 4-byte cold
`__throw_bad_function_call`.

`std::function<R(Args...)>::operator()` takes `Args...` by value and forwards
them as `Args&&`, so every scalar argument is materialized in memory and passed
by address. `ebd` selects `smart_forward_t<Args>`, which degenerates to by-value
for register-passable types, so its invoker stays `Ret (*)(ErasurePass, int, int)`.

### Summary

| | `ebd::classic_fn` | `std::function` |
|---|---|---|
| dispatch levels per invocation | 2 | 2 |
| scalar arguments | registers | stored to stack, address passed, loaded back |
| empty state | `m_invoker` points to `empty::invoke` -> throws `std::bad_function_call` | `_M_empty()` test and branch on every call |
| call site | 6 instructions, 24 bytes | 13 instructions, 52 bytes + 4 cold |
| `sizeof` / `alignof` | 16 / 8 | 16 / 4 |

GCC emits no empty-state test here: nothing compares `m_invoker` against a
sentinel, and no `empty::invoke` symbol is emitted at all for this source.

### Destruction and copy

```asm
; ebd: NULL third vtable entry, so a trivially destructible payload is not called
    864c:	e5903008 	ldr	r3, [r0, #8]
    8650:	e5933008 	ldr	r3, [r3, #8]
    8654:	e3530000 	cmp	r3, #0
    8658:	e92d4010 	push	{r4, lr}
    865c:	e1a04000 	mov	r4, r0
    8660:	11a0e00f 	movne	lr, pc
    8664:	112fff13 	bxne	r3
    8668:	e1a00004 	mov	r0, r4
    866c:	e8bd4010 	pop	{r4, lr}
    8670:	e12fff1e 	bx	lr

; std::function: _M_manager is always invoked with __destroy_functor
    8620:	e5903008 	ldr	r3, [r0, #8]
    8624:	e3530000 	cmp	r3, #0
    8628:	e92d4010 	push	{r4, lr}
    862c:	e1a04000 	mov	r4, r0
    8630:	13a02003 	movne	r2, #3
    8634:	11a01000 	movne	r1, r0
    8638:	11a0e00f 	movne	lr, pc
    863c:	112fff13 	bxne	r3
    8640:	e1a00004 	mov	r0, r4
    8644:	e8bd4010 	pop	{r4, lr}
    8648:	e12fff1e 	bx	lr
```

The manager vtable installed for a function pointer is:

```asm
        .word	trivially_clone<int (*)(int, int)>   ; `bl memcpy` with size 4
        .word	trivially_move<int (*)(int, int)>
        .word	0                                    ; NULL: nothing to destroy
```
