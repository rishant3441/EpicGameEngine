//
// Created by Rishan Thangaraj on 3/26/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include "EpicGameEngine/Renderer/UniformBuffer.h"

namespace EpicGameEngine
{
    class OpenGLUniformBuffer : public UniformBuffer
    {
    public:
        OpenGLUniformBuffer(uint32_t size, uint32_t binding);
        virtual ~OpenGLUniformBuffer();

        void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
    private:
        uint32_t rendererID = 0;
    };

} // EpicGameEngine