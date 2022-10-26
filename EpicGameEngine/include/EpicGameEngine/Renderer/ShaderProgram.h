//
// Created by Rishan Thangaraj on 9/9/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

namespace EpicGameEngine
{
    class ShaderProgram
    {
    public:
        ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
        ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader,
                                     const std::string& positionName,
                                     const std::string& texCoordName,
                                     const std::string& colorName,
                                     const std::string& mvpName);
        ~ShaderProgram();

        void Bind() const;
        void Unbind() const;

        void SetInt(const std::string& locName, int value) const;
        void SetIntArray(const std::string& locName, int* value, uint32_t amount);
        void SetFloat(const std::string& locName, float value) const;
        void SetFloat3(const std::string& locName, float* values);
        void SetFloat3A(const std::string& locName, float* values);
        void SetFloat4(const std::string& locName, float* values);
        void SetMat4(const std::string& locName, const glm::mat4& matrix);

        const std::string& GetName() { return name; }
        const uint32_t GetShaderID() { return shaderProgramID; }
    private:
        uint32_t fragmentShaderID;
        uint32_t vertexShaderID;
        uint32_t shaderProgramID;

        std::string name;
        std::string positionName, texcoordName, colorName, mvpName;
        GPU_ShaderBlock* shaderBlock;
    };
}

