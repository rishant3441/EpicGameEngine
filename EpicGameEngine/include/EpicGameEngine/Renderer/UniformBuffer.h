//
// Created by Rishan Thangaraj on 3/26/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() = default;

        virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

        static std::shared_ptr<UniformBuffer> Create(uint32_t size, uint32_t binding);
    };

} // EpicGameEngine