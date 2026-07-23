#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "test_function.hpp"

int main() {
  ebd::fn<int(int, int)> f_non_const = ebd_test_free_func_iii_add;
  ebd::fn<int(int, int) const> f_const = ebd_test_free_func_iii_add;

  // fn<Sig> -> fn<Sig const> is invalid.
  f_const = f_non_const; // FAIL
}
