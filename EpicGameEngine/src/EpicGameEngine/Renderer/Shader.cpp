//
// Created by Rishan Thangaraj on 3/25/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/Shader.h"
#include "EpicGameEngine/Renderer/RendererAPI.h"
#include "EpicGameEngine/Renderer/OpenGL/OpenGLShader.h"

namespace EpicGameEngine
{
    std::shared_ptr<Shader> Shader::Create(std::string filepath)
    {
        RendererAPI::Backend currBackend = RendererAPI::GetBackend();
        switch (currBackend)
        {
            case RendererAPI::Backend::None:
                std::cout << "None not supported" << std::endl;
                break;
            case RendererAPI::Backend::OpenGL:
                return std::make_shared<OpenGLShader>(filepath);
        }
    }
} // EpicGameEngine