#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "test_function.hpp"

int main() {
  ebd::fn_ref<int(int, int)> f_non_const = ebd_test_free_func_iii_add;
  ebd::fn_ref<int(int, int) const> f_const = ebd_test_free_func_iii_add;

  // fn_ref<Sig> -> fn_ref<Sig const> is invalid.
  f_const = f_non_const; // FAIL
}
