#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "test_function.hpp"

int main() {
#if defined(__GNUC__) && !defined(__clang__)
  static_assert(false, "");
#else
  // ebd::fn_ref shouldn't be created from nullptr.
  ebd::fn_ref<int()> f = static_cast<int(*)()>(nullptr); // FAIL
  (void)f;
#endif
}
