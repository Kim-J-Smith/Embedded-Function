#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"

int main() {
  ebd::unique_fn<void(), sizeof(void*)> f_unique;
  ebd::fn<void(), 8 * sizeof(void*)> f = std::move(f_unique); // FAIL
}
