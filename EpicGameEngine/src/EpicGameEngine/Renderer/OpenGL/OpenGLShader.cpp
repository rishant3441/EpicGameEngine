//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/OpenGL/OpenGLShader.h"
#include "glew.h"

#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        ShaderProgramSource source = ParseShader(filepath);
        rendererID = CreateShader(source.vertexSource, source.fragmentSource);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(rendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(rendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetUniform4f(const std::string& name, const glm::vec4& value)
    {
        glUniform4f(glGetUniformLocation(rendererID, name.c_str()), value.x, value.y, value.z, value.w);
    }

    ShaderProgramSource OpenGLShader::ParseShader(const std::string& filepath)
    {
        std::ifstream stream(filepath);

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#type") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else if ((int)type >= 0)
            {
                ss[(int)type] << line << '\n';
            }
        }
        return { ss[0].str(), ss[1].str() };
    }


    uint32_t OpenGLShader::CompileShader(uint32_t type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (!result)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*) alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile shader!" << std::endl;
            std::cout << message << std::endl;
        }

        return id;
    }

    uint32_t OpenGLShader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        uint32_t program = glCreateProgram();
        uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
        uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }
}