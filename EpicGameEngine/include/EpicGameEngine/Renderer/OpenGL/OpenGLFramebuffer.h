//
// Created by Rishan Thangaraj on 4/2/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include "EpicGameEngine/Renderer/Framebuffer.h"

namespace EpicGameEngine
{

    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& newSpecification);
        virtual ~OpenGLFramebuffer();

        void Invalidate();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const;

        virtual const FramebufferSpecification& GetSpecification() const override;
    private:
        uint32_t rendererID = 0;
        FramebufferSpecification specification;

        std::vector<FramebufferTextureSpecification> colorAttachmentSpecifications;
        FramebufferTextureSpecification depthAttachmentSpecification = FramebufferTextureFormat::None;

        std::vector<uint32_t> colorAttachments;
        uint32_t depthAttachment = 0;
    };

} // EpicGameEngine