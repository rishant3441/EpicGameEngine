//
// Created by Rishan Thangaraj on 4/2/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/OpenGL/OpenGLFramebuffer.h"

#include <cassert>

#include <glew.h>


namespace EpicGameEngine
{
    static int maxFramebufferSize = 8192;

    static GLenum textureTarget(bool multisampled)
    {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void createTextures(bool multisampled, uint32_t* outID, uint32_t count)
    {
        glCreateTextures(textureTarget(multisampled), count, outID);
    }

    static void bindTexture(bool multisampled, uint32_t id)
    {
        glBindTexture(textureTarget(multisampled), id);
    }

    static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multisampled), id, 0);
    }

    static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
    }

    static bool isDepthFormat(FramebufferTextureFormat format)
    {
        switch (format)
        {
            case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
        }

        return false;
    }

    static GLenum FBTextureFormatToGL(FramebufferTextureFormat format)
    {
        switch (format)
        {
            case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
            case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
        }

        assert(false);
        return 0;
    }

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& newSpecification)
        : specification(newSpecification)
    {
        for (auto spec : specification.attachments.attachments)
        {
            if (!isDepthFormat(spec.textureFormat))
                colorAttachmentSpecifications.emplace_back(spec);
            else
                depthAttachmentSpecification = spec;
        }

        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &rendererID);
        glDeleteTextures(colorAttachments.size(), colorAttachments.data());
        glDeleteTextures(1, &depthAttachment);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);
        glViewport(0, 0, specification.Width, specification.Height);
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > maxFramebufferSize || height > maxFramebufferSize)
        {
            assert(false && "Attempted to resize to a not good value");
            return;
        }
        specification.Width = width;
        specification.Height = height;

        Invalidate();
    }

    int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        assert(attachmentIndex < colorAttachments.size());

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }

    void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        assert(attachmentIndex < colorAttachments.size());

        auto& spec = colorAttachmentSpecifications[attachmentIndex];
        glClearTexImage(colorAttachments[attachmentIndex], 0, FBTextureFormatToGL(spec.textureFormat), GL_INT, &value);
    }

    const FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const
    {
        return specification;
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (rendererID)
        {
            glDeleteFramebuffers(1, &rendererID);
            glDeleteTextures(colorAttachments.size(), colorAttachments.data());
            glDeleteTextures(1, &depthAttachment);

            colorAttachments.clear();
            depthAttachment = 0;
        }

        glCreateFramebuffers(1, &rendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, rendererID);

        bool multisample = specification.Samples > 1;

        // Attachments
        if (colorAttachmentSpecifications.size())
        {
            colorAttachments.resize(colorAttachmentSpecifications.size());
            createTextures(multisample, colorAttachments.data(), colorAttachments.size());

            for (size_t i = 0; i < colorAttachments.size(); i++)
            {
                bindTexture(multisample, colorAttachments[i]);
                switch (colorAttachmentSpecifications[i].textureFormat)
                {
                    case FramebufferTextureFormat::RGBA8:
                        attachColorTexture(colorAttachments[i], specification.Samples, GL_RGBA8, GL_RGBA, specification.Width, specification.Height, i);
                        break;
                    case FramebufferTextureFormat::RED_INTEGER:
                        attachColorTexture(colorAttachments[i], specification.Samples, GL_R32I, GL_RED_INTEGER, specification.Width, specification.Height, i);
                        break;
                }
            }
        }

        if (depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None)
        {
            createTextures(multisample, &depthAttachment, 1);
            bindTexture(multisample, depthAttachment);
            switch (depthAttachmentSpecification.textureFormat)
            {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                    AttachDepthTexture(depthAttachment, specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, specification.Width, specification.Height);
                    break;
            }
        }

        if (colorAttachments.size() > 1)
        {
            assert(colorAttachments.size() <= 4);
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(colorAttachments.size(), buffers);
        }
        else if (colorAttachments.empty())
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << status << " " << glGetError() << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const
    {
        return colorAttachments[index];
    }
} // EpicGameEngine