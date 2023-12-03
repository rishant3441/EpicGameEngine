//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/RendererAPI.h"
#include "EpicGameEngine/Renderer/OpenGL/OpenGLRendererAPI.h"
#include <iostream>

namespace EpicGameEngine
{
    RendererAPI::Backend RendererAPI::backend = RendererAPI::Backend::OpenGL;

    std::unique_ptr<RendererAPI> RendererAPI::Create()
    {
        switch (backend)
        {
            case Backend::None:
                std::cout << "Not supported" << std::endl;
                break;
            case Backend::OpenGL:
                return std::make_unique<OpenGLRendererAPI>();
        }
        assert(false && "Not working");
        return nullptr;
    }
}