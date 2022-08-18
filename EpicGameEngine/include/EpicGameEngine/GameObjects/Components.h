//
// Created by Rishan Thangaraj on 7/7/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Renderer/Texture.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/Renderer/Camera/Camera.h>
#include <EpicGameEngine/UUID.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace EpicGameEngine
{
    struct IDComponent
    {
        UUID ID;

        IDComponent() = default;
        IDComponent(UUID uuid)
            : ID(uuid)
        {

        }
        IDComponent(const IDComponent&) = default;
    };

    /// Holds name of the Game Object
    /**
     *
     *  Holds the name of the GameObject with an std::string, passed in with move semantics.
     *
     * */
    struct NameComponent
    {
        std::string name;

        NameComponent() = default;
        NameComponent(const NameComponent&) = default;
        NameComponent(const std::string& name)
            : name(name)
        {}
    };

    struct TransformComponent
    {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position)
            : Position(position)
        {}

        glm::mat4 GetRotation() const
        {
            return glm::toMat4(glm::quat(Rotation));
        }
        [[nodiscard]] glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Position)
            * rotation
            * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct SpriteRendererComponent
    {
        SDL_Color Color{255, 255, 255, 255};
        std::shared_ptr<Texture> Texture;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const SDL_Color& color)
            : Color(color)
        {}
    };

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true;
        bool fixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    struct CSharpScriptComponent
    {
        std::string name;

        CSharpScriptComponent() = default;
        CSharpScriptComponent(const CSharpScriptComponent&) = default;
    };

    struct NativeScriptComponent
    {
        ScriptableGameObject* Instance = nullptr;

        ScriptableGameObject*(*InstantiateScript)();
        void (*DeleteScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstantiateScript = []() { return static_cast<ScriptableGameObject*>(new T()); };
            DeleteScript = [](NativeScriptComponent* script) { delete script->Instance; script->Instance = nullptr; };
        }
    };
}