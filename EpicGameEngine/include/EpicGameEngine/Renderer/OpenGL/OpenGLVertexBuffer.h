//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include "EpicGameEngine/Renderer/VertexBuffer.h"

namespace EpicGameEngine
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* verticies, uint32_t size);
        virtual ~OpenGLVertexBuffer();

        void Bind() override;
        void Unbind() override;

        void SetData(const void* data, uint32_t size) override;
        void SetLayout(const BufferLayout& layout) override;

        const BufferLayout& GetLayout() const override { return layout; }

    private:
        uint32_t rendererID;
        BufferLayout layout;
    };
}
