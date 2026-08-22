**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- Added an `Alignment` template parameter to polymorphic function wrappers, which changes its template argument list. Existing code that directly instantiates `basic_fn` is affected; other wrappers (`ebd::fn`, `ebd::unique_fn`, etc.) are unaffected as the new parameter has a default value. (#147)

**✨ New Features**
- Added the `Alignment` template parameter to specify the alignment of the internal storage. (#147)
- `make_fn` now automatically deduces the alignment from the callable object. (#147)

**🛠️ Optimizations and Improvements**
- None.

**📌 Notes**
- None.
