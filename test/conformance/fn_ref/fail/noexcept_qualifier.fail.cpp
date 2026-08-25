#include "test_function.hpp"

int main() {
#if __cpp_noexcept_function_type >= 201510L
  ebd::fn_ref<int() noexcept> f_noexcept = ebd_test_free_func_noexcept;
  ebd::fn_ref<int() noexcept(false)> f_maythrow = ebd_test_free_func_maythrow;

  f_noexcept = f_maythrow; // FAIL
#else
  static_assert(false, "FAIL"); // FAIL
#endif
}
