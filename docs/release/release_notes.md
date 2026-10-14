**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- All owning function wrappers (`fn`, `unique_fn`, etc.) now reject callable objects whose destructor may thow exceptions.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- The destructor of all owning function wrappers are declare as `noexcept` now.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
