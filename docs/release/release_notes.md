**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
- `make_fn(std::cw<...>)` and `make_fn(std::cw<...>, obj)` are deprecated. Their API will be changed in v2.4.0; use `make_fn<ebd::fn_ref>(std::cw<...>)` / `make_fn<ebd::fn_ref>(std::cw<...>, obj)` instead.
