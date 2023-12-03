//
// Created by Rishan Thangaraj on 3/26/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/UniformBuffer.h"
#include "EpicGameEngine/Renderer/OpenGL/OpenGLUniformBuffer.h"
#include "EpicGameEngine/Renderer/RendererAPI.h"

namespace EpicGameEngine
{
    std::shared_ptr<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        RendererAPI::Backend currBackend = RendererAPI::GetBackend();
        switch (currBackend)
        {
            case RendererAPI::Backend::None:
                std::cout << "None not implemented!" << std::endl;
                break;
            case RendererAPI::Backend::OpenGL:
                return std::make_shared<OpenGLUniformBuffer>(size, binding);
        }
        return nullptr;
    }
} // EpicGameEngine