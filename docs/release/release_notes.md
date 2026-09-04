**🔧 Fixed Bugs**
- Fixed a bug where `make_fn` would be `noexcept(false)` when the passed-in functor is not nothrow-destructible. (#166)
- Fixed a bug where assigning `fn<Sig, Small>` to `fn<Sig, Large>` would lose empty state. (#168)

**⚠️ Breaking Changes**
- `make_fn(std::cw<...>)` and `make_fn(std::cw<...>, ...)` now return `fn` instead of `fn_ref`. If you still want them to return `fn_ref`, use `make_fn<fn_ref>(std::cw<...>)` and `make_fn<fn_ref>(std::cw<...>, ...)` instead. (#160)

**✨ New Features**
- Owning polymorphic function wrappers can now be constructed from `{std::cw<...>, obj}` since C++26, as an **experimental** exploration of [P2511: Beyond operator(): NTTP callables in type-erased call wrappers](https://wg21.link/P2511). (#160)

**🛠️ Optimizations and Improvements**
- Owning polymorphic function wrappers no longer zero-initialize `m_erasure` when constructing from a functor, in order to reduce binary size. (#164)
- Default-constructed function wrappers (and those constructed from `nullptr`) no longer zero-initialize the whole internal storage and command table; only `sizeof(char)` (one byte) is written into `m_erasure` to suppress GCC warnings (`-Wmaybe-uninitialized` / `-Wuninitialized`), and the buffer-converting copy/move constructors likewise now write one byte instead of `sizeof(void*)`, in order to reduce binary size and initialization cost. (#165)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
- The API of `make_fn(std::cw<...>)` and `make_fn(std::cw<...>, obj)` **have been changed**.
