#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"

int main() {
  ebd::fn<void(), sizeof(void*)> f_normal;
  ebd::classic_fn<void(), sizeof(void*)> f = f_normal; // FAIL
}
