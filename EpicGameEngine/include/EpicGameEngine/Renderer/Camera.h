//
// Created by Rishan Thangaraj on 3/26/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <glm/glm.hpp>
#include <EpicGameEngine/ege_pch.h>

#include <EpicGameEngine/Input/Input.h>
#include <EpicGameEngine/Timestep.h>

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
        float perspectiveVerticalFOV = 9900000256.000f;
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


        float GetNearClip() { return this->projectionType == Camera::ProjectionType::Orthographic ? orthographicNear : perspectiveNear; }
        float GetFarClip() { return this->projectionType == Camera::ProjectionType::Orthographic ? orthographicFar : perspectiveFar; }

        void SetOrthographic(float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);

        void SetNearClip(float nearClip);
        void SetFarClip(float farClip);
        void SetPerspectiveFOV(float fov);

        float width, height;

    private:
        void recalculateProjection();
    };

    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        inline void SetViewportSize(float width, float height)
        {
            viewportWidth = width;
            viewportHeight = height;
            UpdateProjection();
        }

        glm::vec3 CalculatePosition() const;

        float GetDistance() const { return distance; }
        float SetDistance(float newDistance) { distance = newDistance; }

        const glm::mat4& GetViewMatrix() const
        {
            return viewMatrix;
        }

        const glm::mat4 GetProjectionMatrix() const
        {
            return projection;
        }

        glm::mat4 GetViewProjectionMatrix() const { return projection * viewMatrix; }

        const glm::vec3& GetPosition() const
        {
            return position;
        }

        float GetPitch() const
        {
            return pitch;
        }

        float GetYaw() const
        {
            return yaw;
        }

        glm::vec3 GetForwardDirection() const;
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetUpDirection() const;
        glm::quat GetOrientation() const;
        bool OnMouseScroll(MouseScrolledEvent& e);

        void OnUpdate(Timestep timestep);
        void OnEvent(std::shared_ptr<Event> e);

    private:

        void UpdateProjection();
        void UpdateView();
        void MouseZoom(float delta);
        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);

        float ZoomSpeed();
        std::pair<float, float> PanSpeed() const;


        float fov = 45.0f, aspectRatio = 1.778f, nearClip = 0.1f, farClip = 1000.0f;

        glm::mat4 viewMatrix;
        glm::vec3 position = { 0.0f, 0.0f, 1.0f };
        glm::vec3 FocalPoint = { 0.0f, 0.0f, 0.0f };

        glm::vec2 initialMousePos = { 0.0f, 0.0f };

        float distance = 10.0f;
        float pitch = 0.0f, yaw = 0.0f;

        float viewportWidth = 854, viewportHeight = 576;

    };
}
