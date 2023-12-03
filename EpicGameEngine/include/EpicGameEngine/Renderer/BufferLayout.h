//
// Created by Rishan Thangaraj on 3/24/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/ege_pch.h>
#include "glew.h"

namespace EpicGameEngine
{
    enum class DataTypes
    {
       None = 0,
       Float,
       Float2,
       Float3,
       Float4,
       Mat3,
       Mat4,
       Int,
       Int2,
       Int3,
       Int4,
       Bool
    };

    static uint32_t DataTypeSize(DataTypes type)
    {
        switch (type)
        {
            case DataTypes::Float:    return 4;
            case DataTypes::Float2:   return 4 * 2;
            case DataTypes::Float3:   return 4 * 3;
            case DataTypes::Float4:   return 4 * 4;
            case DataTypes::Mat3:     return 4 * 3 * 3;
            case DataTypes::Mat4:     return 4 * 4 * 4;
            case DataTypes::Int:      return 4;
            case DataTypes::Int2:     return 4 * 2;
            case DataTypes::Int3:     return 4 * 3;
            case DataTypes::Int4:     return 4 * 4;
            case DataTypes::Bool:     return 1;
            case DataTypes::None:
                break;
        }
        return 0;
    }

    struct BufferElement
    {
        BufferElement() = default;
        BufferElement(DataTypes type, const std::string& name, bool normalized = false)
            : name(name), type(type), size(DataTypeSize(type)), offset(0), normalized(normalized) {}

        std::string name;
        DataTypes type;
        uint32_t count;
        uint32_t size;
        size_t offset;
        bool normalized;

        static uint32_t GetSizeOfType(uint32_t type)
        {
            switch (type)
            {
                case GL_FLOAT: return 4;
                case GL_UNSIGNED_INT: return 4;
                case GL_UNSIGNED_BYTE: return 1;
                default: return 4;
            }
            return 0;
        }
        uint32_t GetComponentCount() const
        {
            switch (type)
            {
                case DataTypes::Float:   return 1;
                case DataTypes::Float2:  return 2;
                case DataTypes::Float3:  return 3;
                case DataTypes::Float4:  return 4;
                case DataTypes::Mat3:    return 3; // 3* float3
                case DataTypes::Mat4:    return 4; // 4* float4
                case DataTypes::Int:     return 1;
                case DataTypes::Int2:    return 2;
                case DataTypes::Int3:    return 3;
                case DataTypes::Int4:    return 4;
                case DataTypes::Bool:    return 1;
            }

            assert(false && "Unknown ShaderDataType!");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() = default;
        BufferLayout(std::initializer_list<BufferElement> elements)
            : elements(elements)
        {
            CalculateOffsetsAndStride();
        }
        ~BufferLayout() = default;

        inline const std::vector<BufferElement>& GetElements() const { return elements; }
        std::vector<BufferElement>::iterator begin() { return elements.begin(); }
        std::vector<BufferElement>::iterator end() { return elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

        inline uint32_t GetStride() const { return stride; }

    private:
        std::vector<BufferElement> elements;
        uint32_t stride = 0;

        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            stride = 0;
            for (auto& element : elements)
            {
                element.offset = offset;
                offset += element.size;
                stride += element.size;
            }
        }
    };

} // EpicGameEngine