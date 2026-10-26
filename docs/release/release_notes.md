**🔧 Fixed Bugs**
- Fixed undefined behavior when an owning wrapper constructor throws. The destructor is now defined in the most-derived wrapper class.
- Added missing header file `<memory>` for `std::addressof`.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Removed the unused internal `crtp_mixins::destructor_impl` base and the view specialization of `crtp_mixins::lifetime_operations_impl`. Owning wrappers now declare their destructor directly, and view wrappers no longer inherit a lifetime-operations base. No user-visible API change.
- Removed unnecessary internal traits and adjusted some comments' position.

**📌 Notes**
- `operator bool` still works but may warn. It will be removed in a future release.
