**🔧 Fixed Bugs**
- Fixed potential undefined behavior caused by treating functors that are both trivial and empty as *stateless*.

**⚠️ Breaking Changes**
- Removed the `EMBED_FN_CONFIG_EMPTY_TRIVIAL_STATEFUL` macro. Empty trivial functors are now always treated as stateful: owning wrappers store them, which may increase wrapper size but keeps the `this` pointer stable across invocations.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Added `[[msvc::intrinsic]]` to the internal function to enhance the performance in `Debug` build mode.
- Optimized benchmark case `StdOperatorWrapper.FunctionWrapperAsParams`.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
