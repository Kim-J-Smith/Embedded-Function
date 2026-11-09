**🔧 Fixed Bugs**
- None.

**⚠️ Breaking Changes**
- None.

**✨ New Features**
- None.

**🛠️ Optimizations and Improvements**
- Added `aligned_memcpy` to preserve pointer alignment and avoid byte-by-byte copies on strict-alignment targets such as RISC-V, using it only for trivial clones where it is actually needed.
- CI now cancels superseded runs on the same pull request to save runner resources, while runs triggered by pushes or manual dispatches are not canceled.

**📌 Notes**
- None.
