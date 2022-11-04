//
// Created by Rishan Thangaraj on 10/28/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/CoreLib/RNG.h>

namespace CoreLib
{

    uint32_t RNG::random()
    {
        return gen();
    }

    uint64_t RNG::random_64()
    {
        return gen_64();
    }

    uint32_t RNG::random_range(int lower, int larger)
    {
       std::uniform_real_distribution<double> dist(lower, larger);
       return dist(gen);
    }
}

