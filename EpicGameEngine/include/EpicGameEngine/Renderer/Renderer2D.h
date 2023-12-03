//
// Created by Rishan Thangaraj on 3/25/2023.
//
// Copyright (c) 2023, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/ege_pch.h>
#include "EpicGameEngine/Renderer/Camera.h"

namespace EpicGameEngine
{

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(EditorCamera& camera);
        static void BeginScene(SceneCamera& camera, const glm::mat4& transform);
        static void EndScene();

        static void Flush();

        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

    private:
        static void StartBatch();
        static void NextBatch();
    };

} // EpicGameEngine