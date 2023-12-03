//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/Renderer.h"
#include "EpicGameEngine/Renderer/Renderer2D.h"

namespace EpicGameEngine
{
    std::unique_ptr<RendererAPI> Renderer::rendererAPI = RendererAPI::Create();
    void Renderer::Init()
    {
        rendererAPI->Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        rendererAPI->Shutdown();
    }

    void Renderer::DrawIndicies(const std::shared_ptr<VertexArray>& va, uint32_t indexCount)
    {
        rendererAPI->DrawIndicies(va, indexCount);
    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        rendererAPI->SetViewport(x, y, width, height);
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        rendererAPI->SetClearColor(color);
    }

    void Renderer::Clear()
    {
        rendererAPI->Clear();
    }
}
