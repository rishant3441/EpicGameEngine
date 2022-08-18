//
// Created by Rishan Thangaraj on 8/8/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/UUID.h>

#include <random>

namespace EpicGameEngine
{
    static std::random_device randomDevice;
    static std::mt19937_64 randomEngine(randomDevice());
    static std::uniform_int_distribution<uint64_t> uniformDistribution;

    UUID::UUID()
        : uuid(uniformDistribution(randomEngine))
    {

    }

    UUID::UUID(uint64_t uuid)
        : uuid(uuid)
    {

    }
}