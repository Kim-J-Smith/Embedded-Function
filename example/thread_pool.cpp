/// thread_pool.cpp — simple thread pool using ebd::unique_fn for move-only task storage
#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <vector>
#include <queue>
#include <condition_variable>
#include <functional>
#include <type_traits>
#include <embed/embed_function.hpp>

class ThreadPool {
private:
    using Task_t = decltype(ebd::make_fn<void()>(std::packaged_task<void()>{}));

    std::vector<std::thread> m_wokers;
    std::queue<Task_t> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cond_var;
    bool m_stop_flag;

#if EMBED_CXX_VERSION >= 201703L
    template <class F, class... Ts> using Result_t = std::invoke_result_t<F, Ts...>;
#else
    template <class F, class... Ts> using Result_t = typename ebd::detail::invoke_result<F, Ts...>::type;
#endif

    void thread_work() {
        while (true) {
            Task_t task;
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cond_var.wait(lock, [this] { return m_stop_flag || !m_tasks.empty(); });
                if (m_stop_flag && m_tasks.empty()) { return; }
                task = std::move(m_tasks.front());
                m_tasks.pop();
            }
            task();
        }
    }
public:
    explicit ThreadPool(std::size_t thread_number = std::thread::hardware_concurrency()) {
        if (thread_number == 0) { thread_number = 1; }
        m_wokers.reserve(thread_number);
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stop_flag = false;
            for (std::size_t i = 0; i < thread_number; i++) {
                m_wokers.emplace_back(&ThreadPool::thread_work, this);
            }
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stop_flag = true;
        }
        m_cond_var.notify_all();
        for (auto& w : m_wokers) { w.join(); }
    }

    template <typename Callable, typename... Args>
    std::future<Result_t<Callable, Args...>> submit(Callable&& callable_obj, Args&&... args) {
        // ebd::unique_fn wraps move-only packaged_task - no std::shared_ptr needed
        auto new_task = std::packaged_task<Result_t<Callable, Args...>()>(
            std::bind(std::forward<Callable>(callable_obj), std::forward<Args>(args)...));
        auto result = new_task.get_future();
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_tasks.emplace(std::move(new_task));
        }
        m_cond_var.notify_one();
        return result;
    }
};

/*
=== Typical output:
Hello! I'm task 1;
Task 4 return: Hello! I'm task 2;
Hello! I'm task 3;
Hello! I'm task 4;
42
===
*/

int main() {
    ThreadPool tp;
    tp.submit([]{ std::cout << "Hello! I'm task 1;\n"; });
    tp.submit([]{ std::cout << "Hello! I'm task 2;\n"; });
    tp.submit([]{ std::cout << "Hello! I'm task 3;\n"; });
    auto res = tp.submit([]{ std::cout << "Hello! I'm task 4;\n"; return 42; });
    std::cout << "Task 4 return: " << res.get() << '\n';
}
