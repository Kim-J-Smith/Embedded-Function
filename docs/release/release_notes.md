**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- `make_fn(std::cw<...>)` and `make_fn(std::cw<...>, ...)` now return `fn` instead of `fn_ref`. If you still want them to return `fn_ref`, use `make_fn<fn_ref>(std::cw<...>)` and `make_fn<fn_ref>(std::cw<...>, ...)` instead.

**✨ New Features**
- Owning polymorphic function wrappers can now be constructed from `{std::cw<...>, obj}` since C++26, as an **experimental** exploration of [P2511: Beyond operator(): NTTP callables in type-erased call wrappers](https://wg21.link/P2511).

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
