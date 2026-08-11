#include "test_fallback_macros.hpp"
#include "embed/embed_function.hpp"

int main() {
    {
        auto f = ebd::make_fn<ebd::fn>([]{});
        f();
    }
    {
        auto f = ebd::make_fn<ebd::unique_fn>([]{});
        f();
    }
    {
        auto f = ebd::make_fn<ebd::classic_fn>([]{});
        f();
    }
    {
        auto f = ebd::make_fn<ebd::fn_ref>(+[]{});
        f();
    }
    return 0;
}

