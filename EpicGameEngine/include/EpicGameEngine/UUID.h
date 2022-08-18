//
// Created by Rishan Thangaraj on 8/8/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/ege_pch.h>
#include <xhash>

namespace EpicGameEngine
{
    class UUID
    {
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const { return uuid; }
    private:
        uint64_t uuid;
    };
}

namespace std
{
    template<>
    struct hash<EpicGameEngine::UUID>
    {
        std::size_t  operator()(const EpicGameEngine::UUID& uuid) const
        {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
}

