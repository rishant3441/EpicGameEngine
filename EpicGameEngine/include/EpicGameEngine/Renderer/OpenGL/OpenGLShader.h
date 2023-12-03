//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include "EpicGameEngine/Renderer/Shader.h"

#include <string>

namespace EpicGameEngine
{
    struct ShaderProgramSource
    {
        std::string vertexSource;
        std::string fragmentSource;
    };

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;

        void SetUniform4f(const std::string& name, const glm::vec4& value) override;
    private:
        std::string filePath;
        uint32_t rendererID;

        uint32_t CompileShader(uint32_t type, const std::string& source);
        uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        ShaderProgramSource ParseShader(const std::string& filepath);
    };
}