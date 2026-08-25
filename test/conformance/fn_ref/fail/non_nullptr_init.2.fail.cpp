#include "../__constant_wrapper.hpp"
#include "test_function.hpp"

int main() {
#if __cpp_lib_constant_wrapper < 202603L
  static_assert(false, "");
#else
  // ebd::fn_ref shouldn't be created from std::cw<nullptr>.
  ebd::fn_ref<int()> f(std::cw<static_cast<int(*)()>(nullptr)>); // FAIL
  (void)f;
#endif
}
