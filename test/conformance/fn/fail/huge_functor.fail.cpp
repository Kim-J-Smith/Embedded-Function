#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "test_function.hpp"

struct huge_callable_functor {
  char pod[256];
  int operator()(int a, int b) { return a + b; }
};

int main() {
  huge_callable_functor huge_callable{};
  // sizeof(huge_callable) > ebd::fn<int(int, int)>::get_buffer_size()
  ebd::fn<int(int, int)> f = huge_callable; // FAIL
}
