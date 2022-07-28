//
// Created by Rishan Thangaraj on 7/21/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine.h>
#include <EpicGameEngine/GameObjects/Scene.h>

namespace EpicGameEngine
{
    class GameObjectsPanel
    {
    public:
        GameObjectsPanel() = default;
        GameObjectsPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender();

    private:
        void DrawGameObjectNode(GameObject gameObject);
        void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 75.0f);
        void DrawComponents(GameObject selection);

        Ref<Scene> context;
        GameObject selectionContext;
    };
}

