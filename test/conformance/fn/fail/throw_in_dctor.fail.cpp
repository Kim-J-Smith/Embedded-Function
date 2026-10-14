#include "test_function.hpp"

struct ThrowInDestroy {
    ThrowInDestroy() = default;
    ~ThrowInDestroy() noexcept(false) {}
    void operator()() {}
};

int main() {
  // FAIL: callable object shouldn't throw in destructor.
  ebd::fn<void()> f = ThrowInDestroy{}; // FAIL
}
