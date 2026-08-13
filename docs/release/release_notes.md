**🔧 Fixed Bugs**
- Fixed freestanding support: `throw_or_terminate()` now uses `std::terminate()` and the `std::function` overload of `not_empty()` is skipped when `__STDC_HOSTED__` is not defined. (#132)
- Fixed GCC 16 ipa-cp devirtualization failure by adding `gcc_ipa_cp_friendly_cast()` so indirect calls via `m_invoker` are tracked correctly (see issue #133). (#134)
- Fixed `-Wuninitialized` warning by explicitly initializing `Base_MemberVariable` in the default/`nullptr_t`/functor constructors. (#136)
- Fixed the benchmark bug in `MoveOnlyFunction.Params.SmallTrivial` (wrong `volatile void*` usage). (#137)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- Added [ngcpp/proxy](https://github.com/ngcpp/proxy) as a benchmark target, with `proxy` cases added to all runtime benchmark suites (headers vendored in `benchmark/runtime/proxy/`). (#137)
- Added an ADL swap() free function for exchanging two same-type wrappers. (#128)

**🛠️ Optimizations and Improvements**
- Constructed `ErasurePass` directly with const pointers, adding dedicated constructors for const/volatile-qualified erased pointers. (#134)
- Simplified `operator()` by inlining the invoker call and dropping the `invoke()` helper from the command tables and removing unnecessary `const_cast`. (#134)
- Benchmarks now build with `-O2` (was `-Os`) to avoid the strange bug in MinGW, and benchmark parameters were adjusted (`BENCHMARK_TIMES {1000, 1000000}`, `BENCHMARK_REPEAT 15`). (#137)
- Updated benchmark CI to GCC-16 on Ubuntu 26.04, and README benchmark results (GCC-16, C++23, `-O2`). (#137)
- Enabled sanitizers in the test suite (non-Windows): GCC tests build with `-fsanitize=address` and Clang tests build with `-fsanitize=address,undefined`. (#130)
- Defining `DEBUG` now forces the debug diagnostics (assertions and terminate checks) to stay active even when `NDEBUG` or `__OPTIMIZE__` is defined. (#136)
- Added `[[msvc::intrinsic]]` to `gcc_ipa_cp_friendly_cast()` to help MSVC inline. (#135)

**📌 Notes**
- Benchmark now requires C++20 because of the `proxy` dependency.