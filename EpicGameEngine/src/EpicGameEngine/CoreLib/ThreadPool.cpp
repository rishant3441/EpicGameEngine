//
// Created by Rishan Thangaraj on 10/26/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/CoreLib/ThreadPool.h>
#include <thread>

namespace CoreLib
{
    void ThreadPool::start()
    {
        const uint32_t num_threads = std::thread::hardware_concurrency();
        threads.resize(num_threads);
        for (uint32_t i = 0; i < num_threads; i++)
        {
            threads.at(i) = std::thread(&ThreadPool::thread_loop, this);
        }
    }

    void ThreadPool::thread_loop()
    {
        while (true)
        {
            std::function<void()> job;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                mutex_condition.wait(lock, [this] {
                    return !jobs.empty() || should_terminate;
                })
                if (should_terminate)
                    return;
                job = jobs.front();
                jobs.pop();
            }
        }
    }

    void ThreadPool::queue_job(const std::function<void()>& job)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            jobs.push(job);
        }
        mutex_condition.notify_one();
    }

    bool ThreadPool::busy()
    {
        bool poolbusy;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            poolbusy = jobs.empty();
        }
        return poolbusy;
    }

    void ThreadPool::stop()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            should_terminate = true;
        }
        mutex_condition.notify_all();
        for (std::thread& active_thread : threads)
        {
            active_thread.join();
        }
        threads.clear();
    }
}