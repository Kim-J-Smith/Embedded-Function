**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- Added an `Alignment` template parameter to polymorphic function wrappers, which changes its template argument list. Existing code that directly instantiates `basic_fn` is affected; other wrappers (`ebd::fn`, `ebd::unique_fn`, etc.) are unaffected as the new parameter has a default value.

**✨ New Features**
- Added the `Alignment` template parameter to specify the alignment of the internal storage.
- `make_fn` now automatically deduces the alignment from the callable object.

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- None.
