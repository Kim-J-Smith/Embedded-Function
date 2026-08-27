/// `fn_ref` as zero-overhead callback param, fn as stored callback
#include <iostream>
#include <vector>
#include <embed/embed_function.hpp>

// fn_ref passed entirely in registers — enables direct tail-call to the target
template <typename T>
void for_each(const std::vector<T>& vec, ebd::fn_ref<void(const T&)> callback) {
    for (typename std::vector<T>::size_type i = 0; i < vec.size(); ++i) {
        callback(vec[i]);
    }
}

// fn as stored callback holder (copyable, heap-free)
struct Button {
    ebd::fn<void()> onClick;

    void click() {
        if (!onClick.is_empty()) { onClick(); }
    }
};

static void print_int(int x) {
    std::cout << '[' << x << "] ";
}

struct PrintStr {
    const char* prefix;
    void operator()(int x) const {
        std::cout << prefix << x << ' ';
    }
};

/*
=== Typical output:
free func: [0] [10] [20] [30] [40]
sum via lambda: 100
functor: *0 *10 *20 *30 *40
Button clicked!
Total so far: 100
===
*/

int main() {
    std::vector<int> nums;
    nums.reserve(5);
    for (int i = 0; i < 5; ++i) { nums.push_back(i * 10); }

    // Free function pointer via fn_ref
    std::cout << "free func: ";
    for_each(nums, ebd::fn_ref<void(const int&)>(&print_int));
    std::cout << '\n';

    // Lambda with capture via fn_ref
    int total = 0;
    for_each(nums, ebd::fn_ref<void(const int&)>(
        [&total](const int& v) { total += v; }));
    std::cout << "sum via lambda: " << total << '\n';

    // Functor via fn_ref
    std::cout << "functor: ";
    for_each(nums, ebd::fn_ref<void(const int&)>(PrintStr{"*"}));
    std::cout << '\n';

    // fn as stored callback
    Button btn;
    btn.onClick = [] { std::cout << "Button clicked!\n"; };
    btn.click();

    btn.onClick = [&total] { std::cout << "Total so far: " << total << '\n'; };
    btn.click();

    return 0;
}
