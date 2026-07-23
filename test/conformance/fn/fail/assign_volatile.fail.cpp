#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "test_function.hpp"

int main() {
  ebd::fn<int(int, int)> f_non_volatile = ebd_test_free_func_iii_add;
  ebd::fn<int(int, int) volatile> f_volatile = ebd_test_free_func_iii_add;

  // fn<Sig> -> fn<Sig volatile> is invalid.
  f_volatile = f_non_volatile; // FAIL
}
