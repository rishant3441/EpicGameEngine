//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <cstdint>
#include <memory>
#include "EpicGameEngine/Renderer/BufferLayout.h"

namespace EpicGameEngine
{

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        static std::shared_ptr<VertexBuffer> Create(uint32_t size);
        static std::shared_ptr<VertexBuffer> Create(float* verticies, uint32_t size);

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        virtual const BufferLayout& GetLayout() const = 0;
    };

} // EpicGameEngine