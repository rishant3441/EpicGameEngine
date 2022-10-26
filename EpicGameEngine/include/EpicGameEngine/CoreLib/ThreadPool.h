//
// Created by Rishan Thangaraj on 10/26/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/ege_pch.h>

namespace CoreLib
{
    class ThreadPool
    {
    public:
        void start();
        void queue_job(const std::function<void()>& job);
        void stop();
        bool busy();
    private:
        void thread_loop();

        bool should_terminate = false;
        std::mutex queue_mutex;
        std::condition_variable mutex_condition;
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> jobs;
    };
}

