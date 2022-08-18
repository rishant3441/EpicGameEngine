//
// Created by Rishan Thangaraj on 7/19/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/Renderer/Camera/Camera.h>
#include <EpicGameEngine/Timestep.h>
#include <EpicGameEngine/Renderer/Renderer.h>
#include <EpicGameEngine/Input/Input.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <spdlog/spdlog.h>

namespace EpicGameEngine
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
    {
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        projectionMatrix = glm::ortho(left, right, top, bottom, -1.0f, 1.0f);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void OrthographicCamera::recalculateViewMatrix()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

        viewMatrix = glm::inverse(transform);
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    SceneCamera::SceneCamera()
    {
        recalculateProjection();
    }

    void SceneCamera::SetNearClip(float nearClip)
    {
        if (projectionType == SceneCamera::ProjectionType::Orthographic)
            orthographicNear = nearClip;
        else
            perspectiveNear = nearClip;
    }

    void SceneCamera::SetFarClip(float farClip)
    {
        if (projectionType == SceneCamera::ProjectionType::Orthographic)
            orthographicFar = farClip;
        else
            perspectiveFar = farClip;
    }

    void SceneCamera::SetOrthographic(float nearClip, float farClip)
    {
        orthographicNear = nearClip;
        orthographicFar = farClip;
        recalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t newWidth, uint32_t newHeight)
    {
        width = newWidth;
        height = newHeight;
        aspectRatio = (float) width / height;

        recalculateProjection();
    }

    void SceneCamera::recalculateProjection()
    {
        left = -width/2;
        right = width/2;
        top = -height/2;
        bottom = height/2;
    }

    void SceneCamera::SetPerspectiveFOV(float fov)
    {
        perspectiveVerticalFOV = fov;
    }

    void EditorCamera::UpdateProjection()
    {
        aspectRatio = viewportWidth / viewportHeight;
        projection = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    }

    void EditorCamera::UpdateView()
    {
        position = CalculatePosition();

        glm::quat orientation = GetOrientation();
        viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(orientation);
        viewMatrix = glm::inverse(viewMatrix);
    }

    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : fov(fov), aspectRatio(aspectRatio), nearClip(nearClip), farClip(farClip)
    {
       UpdateView();
    }

    void EditorCamera::OnUpdate(Timestep ts)
    {
        if (Input::isKeyPressed(Keyboard::LeftAlt))
        {
            const glm::vec2 mouse{ Input::GetMouseX(), Input::GetMouseY() };
            glm::vec2 delta = (mouse - initialMousePos) * 0.0045f;
            initialMousePos = mouse;

            if (Input::isMouseButtonPressed(Mouse::ButtonMiddle))
            {
                MousePan(delta);
            }
        }
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f)); // potentially have ot change this
    }

    glm::vec3 EditorCamera::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f)); // potentially have ot change this
    }

    glm::vec3 EditorCamera::GetUpDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f)); // potentially have ot change this
    }

    glm::quat EditorCamera::GetOrientation() const
    {
        return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
    }

    glm::vec3 EditorCamera::CalculatePosition() const
    {
        return FocalPoint - GetForwardDirection() * distance;
    }

    float EditorCamera::ZoomSpeed()
    {
        float newDistance = distance * 0.3f;
        newDistance = std::max(newDistance, 0.0f);
        float speed = newDistance * newDistance;
        speed = std::min(speed, 70.0f);
        return speed;
    }

    void EditorCamera::MouseZoom(float delta)
    {
        distance -= delta * ZoomSpeed();
        if (distance < 1.0f)
        {
            FocalPoint += GetForwardDirection();
            distance = 1.0f;
        }
    }

    void EditorCamera::OnEvent(std::shared_ptr<Event> e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(EGE_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
    }

    bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
    {
        float delta = e.yScrolled * 0.3f;
        MouseZoom(delta);
        UpdateView();
        return false;
    }

    std::pair<float, float> EditorCamera::PanSpeed() const
    {
        float x = std::min(viewportWidth / 1000.0f, 2.4f); // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(viewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    void EditorCamera::MousePan(const glm::vec2& delta)
    {
        auto [xSpeed, ySpeed] = PanSpeed();
        FocalPoint += -GetRightDirection() * delta.x * xSpeed * distance;
        FocalPoint += -GetUpDirection() * delta.y * ySpeed * distance;
        UpdateView();
    }
}
