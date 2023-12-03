//
// Created by Rishan Thangaraj on 4/2/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/Framebuffer.h"
#include "EpicGameEngine/Renderer/RendererAPI.h"
#include "EpicGameEngine/Renderer/OpenGL/OpenGLFramebuffer.h"


namespace EpicGameEngine
{
    std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        RendererAPI::Backend currBackend = RendererAPI::GetBackend();
        switch (currBackend)
        {
            case RendererAPI::Backend::None: std::cout << "Not implemented yet!" << std::endl;
            case RendererAPI::Backend::OpenGL:
                return std::make_shared<OpenGLFramebuffer>(spec);
                break;
        }
        assert(false && "Not working");
        return nullptr;
    }
} // EpicGameEngine