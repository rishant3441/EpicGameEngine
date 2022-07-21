//
// Created by Rishan Thangaraj on 7/19/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <glm/glm.hpp>

namespace EpicGameEngine
{
    class Camera
    {
    public:
        Camera() = default;

        Camera(const glm::mat4& newProjection)
            : projection(newProjection)
        {}
        virtual ~Camera() = default;

        [[nodiscard]] const glm::mat4& GetProjection() const { return projection; }

        float GetLeft() const { return left; }
        float GetRight() const { return right; }
        float GetTop() const { return top; }
        float GetBottom() const { return bottom; }

        float orthographicNear = -1.0f, orthographicFar = 1.0f;

    protected:
        float left, right, top, bottom;
        glm::mat4 projection;
    };

    // TODO: Actually use this
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        void SetProjection(float left, float right, float bottom, float top);

        [[nodiscard]] const glm::vec3& GetPosition() const { return position; }
        void SetPosition(const glm::vec3& newPosition) { position = newPosition; }

        [[nodiscard]] float GetRotation() const { return rotation; }
        void SetRotation(float newRotation) { rotation = newRotation; recalculateViewMatrix(); }

        [[nodiscard]] const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; }
        [[nodiscard]] const glm::mat4& GetViewMatrix() const { return viewMatrix; }
        [[nodiscard]] const glm::mat4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }

    private:
        void recalculateViewMatrix();

        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 viewProjectionMatrix;

        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        float rotation = 0.0f;
    };

    class SceneCamera : public Camera
    {
    public:
        SceneCamera();
        ~SceneCamera() override = default;

        void SetOrthographic(float size, float nearClip, float farClip);

        void SetViewportSize(uint32_t width, uint32_t height);

        [[nodiscard]] float GetOrthographicSize() const { return orthographicSize; }
        void SetOrthographicSize(float size) { orthographicSize = size; recalculateProjection(); }

        float width, height;

    private:
        void recalculateProjection();

        float orthographicSize = 10.0f;
        float aspectRatio;

    };
}

