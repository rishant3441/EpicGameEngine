//
// Created by Rishan Thangaraj on 3/25/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include "OpenGLIndexBuffer.h"
#include "EpicGameEngine/Renderer/VertexArray.h"
#include "OpenGLVertexBuffer.h"

namespace EpicGameEngine
{

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib) override;

        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return vertexBuffers; }
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return indexBuffer; }

    private:
        uint32_t rendererID = 0;
        uint32_t index = 0;
        std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
        std::shared_ptr<IndexBuffer> indexBuffer;
    };

} // EpicGameEngine