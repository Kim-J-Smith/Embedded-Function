#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"

int main() {
  ebd::classic_fn<void(), sizeof(void*)> f_classic;
  ebd::unique_fn<void(), sizeof(void*)> f = f_classic; // FAIL
}
