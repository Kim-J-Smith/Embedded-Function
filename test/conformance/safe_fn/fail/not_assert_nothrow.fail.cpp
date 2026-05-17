#include "test_fallback_macros.hpp"

#include "embed/embed_function.hpp"
#include "test_function.hpp"

struct not_assert_nothrowing {
  ~not_assert_nothrowing() noexcept(false) {}
  int operator()(int a, int b) { return a + b; }
};

int main() {
  not_assert_nothrowing not_assert_nothrow{};
  ebd::safe_fn<int(int, int)> f = not_assert_nothrow; // FAIL
}
