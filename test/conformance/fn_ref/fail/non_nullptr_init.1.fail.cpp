#include "test_function.hpp"

int main() {
#if defined(__clang__) && !defined(EBD_TEST_USE_FALLBACK)
  // ebd::fn_ref shouldn't be created from nullptr.
  ebd::fn_ref<int()> f = static_cast<int(*)()>(nullptr); // FAIL
  (void)f;
#else
  static_assert(false, "");
#endif
}
