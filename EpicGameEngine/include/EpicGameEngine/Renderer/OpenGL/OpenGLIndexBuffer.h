//
// Created by Rishan Thangaraj on 3/25/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include "EpicGameEngine/Renderer/IndexBuffer.h"

namespace EpicGameEngine
{

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indicies, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        inline uint32_t GetCount() const override { return count; }
    private:
        uint32_t rendererID;
        uint32_t count;
    };

} // EpicGameEngine