// File name: Multithread.hpp
// Description: Provide Multithreading support for shape processing.
// 
// Time Spent: About 7h
// Challenges: Studying C++ multithreading concepts.
//
// Revision History
// Date: By: Action
// 11/22/2025 Created the Multithread.hpp
// Learning Multithreading in C++

#ifndef MULTITHREAD_HPP
#define MULTITHREAD_HPP

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <deque>
#include <cstddef>

// Generic thread pool / worker queue.
// Template parameters:
//   T   - item type
//   Ptr - pointer/container type used in queue (default: std::shared_ptr<T>)
// Task signature: void(Ptr)
template <typename T, typename Ptr = std::shared_ptr<T>>
class ThreadPool {
public:
    using ItemPtr = Ptr;
    using Task = std::function<void(ItemPtr)>;

    ThreadPool() : running_(false) {}
    ~ThreadPool() {
        stop();
        wait();
        clear();
    }

    // non-copyable
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    // This non-copyable is use to guarantee unique ownership of tasks as copying produce shallow resource and produced undefined behavior.

    // Start worker threads. If task is null, workers do nothing with items (no-op).
    void start(std::size_t nWorkers = std::thread::hardware_concurrency(), Task task = nullptr) {
        if (nWorkers == 0) nWorkers = 1;
        if (running_.exchange(true)) return; // already running

        task_ = task ? std::move(task) : Task([](ItemPtr){ /* default no-op */ });

        workers_.reserve(nWorkers);
        for (std::size_t i = 0; i < nWorkers; ++i) {
            workers_.emplace_back([this]() {
                while (true) {
                    ItemPtr item;
                    {
                        std::unique_lock<std::mutex> lk(mtx_);
                        cv_.wait(lk, [this]{ return !queue_.empty() || !running_.load(); });
                        if (queue_.empty()) {
                            if (!running_.load()) break;
                            else continue;
                        }
                        item = std::move(queue_.front());
                        queue_.pop_front();
                    }
                    try {
                        if (task_) task_(item);
                    } catch (...) {
                        // swallow or log; do not let exception escape worker thread
                    }
                }
            });
        }
    }

    // Enqueue an item (ItemPtr). Thread-safe.
    void enqueue(ItemPtr item) {
        {
            std::lock_guard<std::mutex> lk(mtx_);
            queue_.push_back(std::move(item));
        }
        cv_.notify_one();
    }

    // Convenience: enqueue a raw pointer (ownership transferred into shared_ptr by default Ptr)
    template <typename U = Ptr, typename = std::enable_if_t<std::is_constructible<U, T*>::value>>
    void enqueueRaw(T* raw) {
        enqueue(Ptr(raw));
    }

    // Stop accepting/processing new work. Workers will exit once queue is empty.
    void stop() {
        running_.store(false);
        cv_.notify_all();
    }

    // Wait for all worker threads to exit.
    void wait() {
        for (auto &t : workers_) if (t.joinable()) t.join();
        workers_.clear();
    }

    // Clear queued items (release pointers)
    void clear() {
        std::lock_guard<std::mutex> lk(mtx_);
        queue_.clear();
    }

private:
    std::deque<ItemPtr> queue_;
    std::vector<std::thread> workers_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic<bool> running_;
    Task task_;
};

#endif // MULTITHREAD_HPP