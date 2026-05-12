#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "test_function.hpp"

int main() {
  ebd_test_operator_unambiguous lref_callable{};
  // '&'-qualifier is not allowed for fn_ref.
  ebd::fn_ref<int(int) &> f = lref_callable; // FAIL
}
