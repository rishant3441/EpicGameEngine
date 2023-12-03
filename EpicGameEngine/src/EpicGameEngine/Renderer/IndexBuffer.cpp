//
// Created by Rishan Thangaraj on 3/25/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/IndexBuffer.h"
#include "EpicGameEngine/Renderer/RendererAPI.h"

#include <EpicGameEngine/ege_pch.h>
#include "EpicGameEngine/Renderer/OpenGL/OpenGLIndexBuffer.h"

namespace EpicGameEngine
{

    std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indicies, uint32_t count)
    {
        RendererAPI::Backend currBackend = RendererAPI::GetBackend();
        switch (currBackend)
        {
            case RendererAPI::Backend::None: std::cout << "Not implemented yet!" << std::endl;
            case RendererAPI::Backend::OpenGL:
                return std::make_shared<OpenGLIndexBuffer>(indicies, count);
                break;
        }
        assert(false && "Not working");
        return nullptr;
    }
} // EpicGameEngine