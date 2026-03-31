# ARM GCC Assembly Analysis

## Free function & scalar parameters

### Source code

```cpp
#include "embed/embed_function.hpp"

static int add(int a, int b) { return a + b; }
static int sub(int a, int b) { return a - b; }

auto main() -> int {
	auto ebd_add = ebd::make_fn(add);
	auto ebd_sub = ebd::make_fn(sub);

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
g++.exe -Os -std=c++11
```

```asm
    845c:	00002233 	.word	0x00002233
    8460:	00001122 	.word	0x00001122

	res = ebd_add(0x2233, 0x1122);
    8354:	e58d0004 	str	r0, [sp, #4]
    8358:	e59f2100 	ldr	r2, [pc, #256]	@ 8460 <main+0x17c> ; #arg2
    835c:	e59f10f8 	ldr	r1, [pc, #248]	@ 845c <main+0x178> ; #arg1
    8360:	e59d302c 	ldr	r3, [sp, #44]	@ 0x2c
    8364:	e28d0024 	add	r0, sp, #36	@ 0x24
    8368:	e1a0e00f 	mov	lr, pc
    # call <int ebd::(...)::InvokerImpl<...>(ErasureBase const*, int, int)>
    836c:	e12fff13 	bx	r3
    8388:	e58d0004 	str	r0, [sp, #4]

00008814 <int ebd::(...)::InvokerImpl<...>(ErasureBase const*, int, int)>:
    8814:	e1a03000 	mov	r3, r0
    8818:	e1a00001 	mov	r0, r1
    881c:	e5933000 	ldr	r3, [r3]
    8820:	e1a01002 	mov	r1, r2
    8824:	e12fff13 	bx	r3          # call free function `add`

; --------------------------------------------------------------------------------

	res = std_add(0x2233, 0x1122);
    8370:	e59f30e4 	ldr	r3, [pc, #228]	@ 845c <main+0x178>
    8374:	e58d3008 	str	r3, [sp, #8]    ; #arg1 stack
    8378:	e59f30e0 	ldr	r3, [pc, #224]	@ 8460 <main+0x17c>
    837c:	e58d300c 	str	r3, [sp, #12]   ; #arg2 stack
	; if (_M_empty())
    8380:	e59d3038 	ldr	r3, [sp, #56]	@ 0x38
    8384:	e3530000 	cmp	r3, #0          ; check empty
    838c:	1a000009 	bne	83b8 <$call_add$>
    8390:	ebffff42 	bl	80a0 <std::__throw_bad_function_call()>
$call_add$:
    83b8:	e59d303c 	ldr	r3, [sp, #60]	@ 0x3c
    83bc:	e28d200c 	add	r2, sp, #12     ; #arg2
    83c0:	e28d1008 	add	r1, sp, #8      ; #arg1
    83c4:	e28d0030 	add	r0, sp, #48	@ 0x30
    83c8:	e1a0e00f 	mov	lr, pc
    # call <std::_Function_handler<...>::_M_invoke(std::_Any_data const&, int&&, int&&)>
    83cc:	e12fff13 	bx	r3
    83e8:	e58d0004 	str	r0, [sp, #4]

00008860 <std::_Function_handler<...>::_M_invoke(std::_Any_data const&, int&&, int&&)>:
    8860:	e1a0c001 	mov	ip, r1
    8864:	e5903000 	ldr	r3, [r0]
    8868:	e5921000 	ldr	r1, [r2]
    886c:	e59c0000 	ldr	r0, [ip]
    8870:	e12fff13 	bx	r3          # call free function `add`

```
