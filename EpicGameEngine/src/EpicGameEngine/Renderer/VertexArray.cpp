//
// Created by Rishan Thangaraj on 3/25/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/VertexArray.h"
#include "EpicGameEngine/Renderer/RendererAPI.h"
#include "EpicGameEngine/Renderer/OpenGL/OpenGLVertexArray.h"

namespace EpicGameEngine
{
    std::shared_ptr<VertexArray> VertexArray::Create()
    {
        RendererAPI::Backend currBackend = RendererAPI::GetBackend();
        switch (currBackend)
        {
            case RendererAPI::Backend::None:
                assert(false && "Doesn't exist!");
                break;
            case RendererAPI::Backend::OpenGL:
               return std::make_shared<OpenGLVertexArray>();
               break;
        }
        assert(false && "Not working");
        return nullptr;
    }
} // EpicGameEngine