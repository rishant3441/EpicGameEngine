//
// Created by Rishan Thangaraj on 3/25/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/OpenGL/OpenGLVertexArray.h"

#include <cassert>

namespace EpicGameEngine
{
    static GLenum DataTypeToOpenGLBaseType(DataTypes type)
    {
        switch (type)
        {
            case DataTypes::Float:    return GL_FLOAT;
            case DataTypes::Float2:   return GL_FLOAT;
            case DataTypes::Float3:   return GL_FLOAT;
            case DataTypes::Float4:   return GL_FLOAT;
            case DataTypes::Mat3:     return GL_FLOAT;
            case DataTypes::Mat4:     return GL_FLOAT;
            case DataTypes::Int:      return GL_INT;
            case DataTypes::Int2:     return GL_INT;
            case DataTypes::Int3:     return GL_INT;
            case DataTypes::Int4:     return GL_INT;
            case DataTypes::Bool:     return GL_BOOL;
        }

        assert(false && "Unknown DataType!");
        return 0;
    }
    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &rendererID);
        glBindVertexArray(rendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {

    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(rendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb)
    {
        assert(!vb->GetLayout().GetElements().empty() && "Vertex Buffer has no layout!");

        Bind();
        vb->Bind();

        const auto& layout = vb->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.type)
            {
                case DataTypes::Float:
                case DataTypes::Float2:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index,
                                          element.GetComponentCount(),
                                          DataTypeToOpenGLBaseType(element.type),
                                          element.normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (const void*)element.offset);
                    index++;
                    break;
                }
                case DataTypes::Float3:
                case DataTypes::Float4:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribPointer(index,
                                          element.GetComponentCount(),
                                          DataTypeToOpenGLBaseType(element.type),
                                          element.normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (const void*)element.offset);
                    index++;
                    break;
                }
                case DataTypes::Int:
                case DataTypes::Int2:
                case DataTypes::Int3:
                case DataTypes::Int4:
                case DataTypes::Bool:
                {
                    glEnableVertexAttribArray(index);
                    glVertexAttribIPointer(index,
                                           element.GetComponentCount(),
                                           DataTypeToOpenGLBaseType(element.type),
                                           layout.GetStride(),
                                           (const void*)element.offset);
                    index++;
                    break;
                }
                case DataTypes::Mat3:
                case DataTypes::Mat4:
                {
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(index);
                        glVertexAttribPointer(index,
                                              count,
                                              DataTypeToOpenGLBaseType(element.type),
                                              element.normalized ? GL_TRUE : GL_FALSE,
                                              layout.GetStride(),
                                              (const void*)(element.offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(index, 1);
                        index++;
                    }
                    break;
                }
                default:
                    assert(false && "Unknown DataType!");
            }
        }
        vertexBuffers.push_back(vb);
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib)
    {
        Bind();
        ib->Bind();
        indexBuffer = ib;
    }
} // EpicGameEngine