//
// Created by Rishan Thangaraj on 7/19/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "EpicGameEngine/Renderer/Camera/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <EpicGameEngine/Renderer/Renderer.h>

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
}
