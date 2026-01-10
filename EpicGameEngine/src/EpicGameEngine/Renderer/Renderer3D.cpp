//
// Created by Rishan Thangaraj on 3/25/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/Renderer3D.h"
#include <memory>
#include "EpicGameEngine/Renderer/Renderer.h"
#include "EpicGameEngine/Renderer/VertexArray.h"
#include "EpicGameEngine/Renderer/VertexBuffer.h"
#include "EpicGameEngine/Renderer/Shader.h"
#include "EpicGameEngine/Renderer/UniformBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <EpicGameEngine/Debug.h>
#include <EpicGameEngine/Renderer/Texture.h>
#include <array>

namespace EpicGameEngine
{
    struct QuadVertex
    {
       glm::vec3 position;
       glm::vec4 color;
       glm::vec2 texCoord;
       float texIndex;
       float tilingFactor;

       int entityID;
    };
    struct Renderer3DData
    {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        std::shared_ptr<VertexArray> quadVertexArray;
        std::shared_ptr<VertexBuffer> quadVertexBuffer;
        std::shared_ptr<Shader> quadShader;
        std::shared_ptr<Texture2D> whiteTexture;

        uint32_t quadIndexCount;
        QuadVertex* quadVertexBase = nullptr;
        QuadVertex* quadVertexPtr = nullptr;
        glm::vec4 quadVertexPositions[4];

        std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> textureSlots;
        uint32_t TextureSlotIndex = 1;

        struct CameraData
        {
            glm::mat4 viewProjection;
        };
        CameraData cameraBuffer;
        std::shared_ptr<UniformBuffer> cameraUniformBuffer;
    };

    static Renderer3DData data;

    void Renderer3D::Init()
    {
        GLenum err = glewInit();
        if (err != GLEW_OK)
        {
            std::cout << "GLEW ERROR: " << glewGetErrorString(err) << std::endl;
            Debug::Log::LogError("GLEW ERROR: ", glewGetErrorString(err));
        }
        data.quadVertexArray = VertexArray::Create();

        data.quadVertexBuffer = VertexBuffer::Create(data.MaxVertices * sizeof(QuadVertex));
        data.quadVertexBuffer->SetLayout({
                 { DataTypes::Float3, "a_Position" },
                 { DataTypes::Float4, "a_Color" },
                 { DataTypes::Float2, "a_TexCoord"     },
                 { DataTypes::Float,  "a_TexIndex"     },
                 { DataTypes::Float,  "a_TilingFactor" },
                 { DataTypes::Int,    "a_EntityID"     }
        });
        data.quadVertexArray->AddVertexBuffer(data.quadVertexBuffer);

        data.quadVertexBase = new QuadVertex[data.MaxVertices];
        data.quadVertexPtr = data.quadVertexBase;

        uint32_t* quadIndices = new uint32_t[data.MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        std::shared_ptr<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, data.MaxIndices);
        data.quadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        data.whiteTexture = Texture2D::Create(TextureSpecification());
        uint32_t whiteTextureData = 0xffffffff;
        data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samples[data.MaxTextureSlots];
        for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
        {
            samples[i] = i;
        }

        data.quadShader = Shader::Create("assets/shaders/Renderer3D_Quad.glsl");

        data.textureSlots[0] = data.whiteTexture;

        data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        data.quadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        data.quadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

        data.cameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer3DData::CameraData), 0);
        std::cout << "Renderer3D Initialized!" << std::endl;
    }

    void Renderer3D::BeginScene(EditorCamera& camera)
    {
        data.cameraBuffer.viewProjection = camera.GetViewProjectionMatrix();
        data.cameraUniformBuffer->SetData(&data.cameraBuffer, sizeof(Renderer3DData::CameraData));

        Renderer3D::StartBatch();
    }

    void Renderer3D::BeginScene(SceneCamera& camera, const glm::mat4& transform)
    {
        data.cameraBuffer.viewProjection = camera.GetProjection() * glm::inverse(transform);
        data.cameraUniformBuffer->SetData(&data.cameraBuffer, sizeof(Renderer3DData::CameraData));

        Renderer3D::StartBatch();
    }

    void Renderer3D::StartBatch()
    {
        data.quadIndexCount = 0;
        data.quadVertexPtr = data.quadVertexBase;
    }

    void Renderer3D::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer3D::Flush()
    {
        if (data.quadIndexCount)
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)data.quadVertexPtr - (uint8_t*)data.quadVertexBase);
            data.quadVertexBuffer->SetData(data.quadVertexBase, dataSize);

            for (uint32_t i = 0; i < data.TextureSlotIndex; i++)
            {
                data.textureSlots[i]->Bind(i);
            }

            data.quadShader->Bind();
            Renderer::DrawIndicies(data.quadVertexArray, data.quadIndexCount);
        }
    }

    void Renderer3D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, color);
    }

    void Renderer3D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
        constexpr size_t quadVertexCount = 4;
        const float textureIndex = 0.0f;
        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        const float tilingFactor = 1.0f;
        if (data.quadIndexCount >= Renderer3DData::MaxIndices)
            NextBatch();

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            data.quadVertexPtr->position = transform * data.quadVertexPositions[i];
            data.quadVertexPtr->color = color;
            data.quadVertexPtr->texIndex = textureIndex;
            data.quadVertexPtr->texCoord = textureCoords[i];
            data.quadVertexPtr->entityID = 0; // TODO: Take in entity id
            data.quadVertexPtr->tilingFactor = tilingFactor;
            data.quadVertexPtr++;
        }

        data.quadIndexCount += 6;
    }

    void Renderer3D::EndScene()
    {
        Flush();
    }

    void Renderer3D::Shutdown()
    {
        delete[] data.quadVertexBase;
    }


} // EpicGameEngine