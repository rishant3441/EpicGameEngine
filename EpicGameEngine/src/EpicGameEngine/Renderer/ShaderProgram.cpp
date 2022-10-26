//
// Created by Rishan Thangaraj on 9/9/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/Renderer/ShaderProgram.h>
#include <EpicGameEngine/ege_pch.h>
#include <glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace EpicGameEngine
{

    ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
    {
        shaderBlock = new GPU_ShaderBlock();
        shaderProgramID = GPU_CreateShaderProgram();

        vertexShaderID = GPU_CompileShader(GPU_VERTEX_SHADER, vertexShader.c_str());
        fragmentShaderID = GPU_CompileShader(GPU_FRAGMENT_SHADER, fragmentShader.c_str());

        GPU_AttachShader(shaderProgramID, vertexShaderID);
        GPU_AttachShader(shaderProgramID, fragmentShaderID);
        GPU_LinkShaderProgram(shaderProgramID);

        glValidateProgram(shaderProgramID);
        GPU_DetachShader(shaderProgramID, vertexShaderID);
        GPU_DetachShader(shaderProgramID, fragmentShaderID);

        *shaderBlock = GPU_LoadShaderBlock(shaderProgramID, nullptr, nullptr, nullptr, nullptr);
    }

    // TODO: Rename this
    static const char* StrHelper(const std::string& string)
    {
        return string == "" ? nullptr : string.c_str();
    }

    ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader, const std::string& positionName,
                                 const std::string& texCoordName, const std::string& colorName, const std::string& mvpName)
    {
        shaderBlock = new GPU_ShaderBlock();
        shaderProgramID = GPU_CreateShaderProgram();

        vertexShaderID = GPU_CompileShader(GPU_VERTEX_SHADER, vertexShader.c_str());
        fragmentShaderID = GPU_CompileShader(GPU_FRAGMENT_SHADER, fragmentShader.c_str());

        GPU_AttachShader(shaderProgramID, vertexShaderID);
        GPU_AttachShader(shaderProgramID, fragmentShaderID);
        GPU_LinkShaderProgram(shaderProgramID);

        glValidateProgram(shaderProgramID);
        GPU_DetachShader(shaderProgramID, vertexShaderID);
        GPU_DetachShader(shaderProgramID, fragmentShaderID);

        *shaderBlock = GPU_LoadShaderBlock(shaderProgramID, StrHelper(positionName), StrHelper(texCoordName), StrHelper(colorName), StrHelper(mvpName));
    }
    ShaderProgram::~ShaderProgram()
    {
        delete shaderBlock;
    }

    void ShaderProgram::Bind() const
    {
        GPU_ActivateShaderProgram(shaderProgramID, shaderBlock);
    }

    void ShaderProgram::Unbind() const
    {
        GPU_DeactivateShaderProgram();
    }

    void ShaderProgram::SetInt(const std::string& locName, int value) const
    {
        glUniform1i(GPU_GetUniformLocation(shaderProgramID, locName.c_str()), value);
    }

    void ShaderProgram::SetIntArray(const std::string& locName, int* value, uint32_t amount)
    {
        assert(false && "SetIntArray Not Implemented");
    }

    void ShaderProgram::SetFloat(const std::string& locName, float value) const
    {
        glUniform1f(GPU_GetUniformLocation(shaderProgramID, locName.c_str()), value);
    }

    void ShaderProgram::SetMat4(const std::string& locName, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(GPU_GetUniformLocation(shaderProgramID, locName.c_str()), 1, false, glm::value_ptr(matrix));
    }

    void ShaderProgram::SetFloat3(const std::string& locName, float* values)
    {
        glUniform3fv(GPU_GetUniformLocation(shaderProgramID, locName.c_str()), 3, values);
    }
    void ShaderProgram::SetFloat3A(const std::string& locName, float* values)
    {
        GPU_SetAttributefv(GPU_GetAttributeLocation(shaderProgramID, locName.c_str()), 3, values);
    }
    void ShaderProgram::SetFloat4(const std::string& locName, float* values)
    {
        glUniform4fv(GPU_GetUniformLocation(shaderProgramID, locName.c_str()), 4, values);
    }
}