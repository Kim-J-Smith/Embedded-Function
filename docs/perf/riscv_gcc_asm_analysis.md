# RISC-V GCC Assembly Analysis

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
g++ -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -msave-restore -std=c++11 -Os
```

```asm
    # The instruction reordering optimization of GCC is very aggressive.

	res = ebd_add(0x2233, 0x1122);
   10156:	6605                	lui	a2,0x1
   10158:	6589                	lui	a1,0x2
   1015a:	c22a                	sw	a0,4(sp)
   1015c:	12260613          	addi	a2,a2,290 # 1122 <arg#1 using register>
   10160:	23358593          	addi	a1,a1,563 # 2233 <arg#2 using register>
   10164:	1048                	addi	a0,sp,36
   # call <int ebd::(...)::InvokerImpl<...>(ErasureBase const*, int, int)>
   10166:	9782                	jalr	a5
   1017a:	c22a                	sw	a0,4(sp)

000102e4 <int ebd::(...)::InvokerImpl<...>(ErasureBase const*, int, int)>:
   102e4:	411c                	lw	a5,0(a0)
   102e6:	872e                	mv	a4,a1
   102e8:	853a                	mv	a0,a4
   102ea:	85b2                	mv	a1,a2
   102ec:	8782                	jr	a5      # call free function `add`

; --------------------------------------------------------------------------------

	res = std_add(0x2233, 0x1122);
   10168:	6789                	lui	a5,0x2
   1016a:	23378793          	addi	a5,a5,563 # 2233 <arg#1 using stack>
   1016e:	c43e                	sw	a5,8(sp)  # stack
   10170:	6785                	lui	a5,0x1
   10172:	12278793          	addi	a5,a5,290 # 1122 <arg#2 using stack>
   10176:	c63e                	sw	a5,12(sp) # stack
   10178:	57e2                	lw	a5,56(sp)
   1017a:	c22a                	sw	a0,4(sp)
   1017c:	ef91                	bnez	a5,10198 <$call_add$> # check empty
   1017e:	3f2d                	jal	100b8 <std::__throw_bad_function_call()>
$call_add$:
   10198:	57f2                	lw	a5,60(sp)
   1019a:	0070                	addi	a2,sp,12    # arg2
   1019c:	002c                	addi	a1,sp,8     # arg1
   1019e:	1808                	addi	a0,sp,48
   # call <std::_Function_handler<...>::_M_invoke(std::_Any_data const&, int&&, int&&)>
   101a0:	9782                	jalr	a5
   101b4:	c22a                	sw	a0,4(sp)

00010312 <std::_Function_handler<...>::_M_invoke(std::_Any_data const&, int&&, int&&)>:
   10312:	872e                	mv	a4,a1
   10314:	411c                	lw	a5,0(a0)
   10316:	420c                	lw	a1,0(a2)
   10318:	4308                	lw	a0,0(a4)
   1031a:	8782                	jr	a5          # call free function `add`

```
