**🔧 Fixed Bugs**
- Fixed undefined behavior when an owning wrapper constructor throws. The destructor is now defined in the most-derived wrapper class. (#245)
- Added missing header file `<memory>` for `std::addressof`. (#247)
- Fixed a bug that caused `make_fn<Sig>(variadic_function)` to trigger a hard error. (#248)
- Fixed potential undefined behavior in the trivial clone path: the address of the empty `erasure_base_t` subobject may differ from the derived erasure object, so `memcpy` now operates on the derived type instead of the base pointer. (#251)

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Removed the unused internal `crtp_mixins::destructor_impl` base and the view specialization of `crtp_mixins::lifetime_operations_impl`. Owning wrappers now declare their destructor directly, and view wrappers no longer inherit a lifetime-operations base. No user-visible API change. (#245)
- Removed redundant internal traits and adjusted some comments' position. (#246)
- MSVC: Annotated the `fn_ref` callable constructor with `[[msvc::lifetimebound]]`. (#249)
- MSVC: Added the `_Notnull_` SAL annotation from `<sal.h>` to the `fn_ref` function-pointer constructor. (#249)
- Copyable function wrappers can now wrap callable objects with a deleted move constructor. (#250)
- The `fn_ref` conversion constructor now uses `sizeof(erasure_type::ErasureRefStorage)` instead of `default_values::non_owning::buffer_size` when copying the stored reference. No user-visible API change. (#251)

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
