#ifndef SIMPLE_MT_H
#define SIMPLE_MT_H

#include <thread>
#include <mutex>
#include <vector>
#include <functional>

template <class T, class U>
struct SimpleMTContext
{
    SimpleMTContext(const std::vector<T> &src)
        : data(src)
        , it(0)
        , completed(false)
    {
        res.resize(src.size());
    }

    const std::vector<T> data;
    std::vector<U> res;
    volatile typename std::vector<T>::size_type it;
    volatile bool completed;
    std::mutex m;
};

template <class T, class U>
struct SimpleMT
{
    static std::vector<U> map(const std::vector<T> &data, std::function<U(const T&)> map_fn, int num_threads = 8)
    {
        SimpleMTContext<T, U> context(data);
        
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; i++) {
            std::thread thread([&context, map_fn]() {
                thread_fn(context, map_fn);
            });
            threads.push_back(std::move(thread));
        }

        for (auto &thread : threads) {
            thread.join();
        }

        return context.res;
    }

    static void thread_fn(SimpleMTContext<T, U> &context, std::function<U(const T&)> map_fn)
    {
        while (!context.completed) {
            std::unique_lock<std::mutex> mutex(context.m);
            if (context.it >= context.data.size()) {
                context.completed = true;
                break;
            }

            auto it = context.it++;
            mutex.unlock();
            context.res[it] = map_fn(context.data[it]);
        }
    }
};

#endif
