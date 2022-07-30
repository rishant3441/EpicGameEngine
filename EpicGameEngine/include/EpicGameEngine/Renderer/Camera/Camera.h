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
        float perspectiveNear = 1.0f, perspectiveFar = 1000.0f;
        float perspectiveVerticalFOV = glm::radians(45.0f);
        float aspectRatio;

        enum class ProjectionType { Perspective = 0, Orthographic = 1};
        ProjectionType projectionType = ProjectionType::Orthographic; /// \brief Sets the projection type

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


        float GetNearClip() { return orthographicNear; }
        float GetFarClip() { return orthographicFar; }

        void SetOrthographic(float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);

        void SetNearClip(float nearClip);
        void SetFarClip(float farClip);
        void SetPerspectiveFOV(float fov);

        float width, height;

    private:
        void recalculateProjection();
    };
}
