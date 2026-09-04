#include "test_function.hpp"

namespace {
    
struct NothrowInCreateButThrowInDestroy {
    NothrowInCreateButThrowInDestroy() noexcept = default;
    ~NothrowInCreateButThrowInDestroy() noexcept(false) {}

    int operator()(int) const { return 42; }
};

using A = NothrowInCreateButThrowInDestroy;

}

using Wrapper = decltype(ebd::make_fn<ebd::fn>(A{}));
// Assert ebd::make_fn<ebd::fn, void, A&> is `noexcept`.
static constexpr Wrapper (*pmake) (A&) noexcept = &ebd::make_fn<ebd::fn, void, A&>;

TEST(Conformance_fn, noexcept_make_fn_pass) {
    (void)pmake;
    SUCCEED();
}

