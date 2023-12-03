//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/VertexBuffer.h"
#include "EpicGameEngine/Renderer/RendererAPI.h"
#include "EpicGameEngine/Renderer/OpenGL/OpenGLVertexBuffer.h"

namespace EpicGameEngine
{
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        RendererAPI::Backend currBackend = RendererAPI::GetBackend();
        switch (currBackend)
        {
            case RendererAPI::Backend::None:
                std::cout << "Not supported!" << std::endl;
                break;
            case RendererAPI::Backend::OpenGL:
                return std::make_shared<OpenGLVertexBuffer>(size);
        }
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* verticies, uint32_t size)
    {
        RendererAPI::Backend currBackend = RendererAPI::GetBackend();
        switch (currBackend)
        {
            case RendererAPI::Backend::None:
                std::cout << "Not supported!" << std::endl;
                break;
            case RendererAPI::Backend::OpenGL:
                return std::make_shared<OpenGLVertexBuffer>(verticies, size);
        }
    }
} // EpicGameEngine