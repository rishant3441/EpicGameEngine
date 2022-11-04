//
// Created by Rishan Thangaraj on 10/28/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/ege_pch.h>

namespace CoreLib
{
    class RNG
    {
    public:
        RNG() = default;

        uint32_t random();
        uint64_t random_64();

        uint32_t random_range(int lower, int larger);
    private:
        std::random_device rd;

        std::mt19937 gen = std::mt19937(rd());
        std::mt19937_64 gen_64 = std::mt19937_64(rd());
    };
}

