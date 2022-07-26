//
// Created by Rishan Thangaraj on 7/21/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "GameObjectsPanel.h"
#include <EpicGameEngine.h>
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace EpicGameEngine
{

    GameObjectsPanel::GameObjectsPanel(const Ref<Scene>& scene)
    {
        SetContext(scene);
    }

    void GameObjectsPanel::SetContext(const Ref<Scene> &scene)
    {
        context = scene;
        selectionContext = {};
    }

    void GameObjectsPanel::OnImGuiRender()
    {
        ImGui::Begin("Game Objects");

        context->registry.each([&](auto gameObjectID)
        {
            GameObject gameObject{ gameObjectID, context.get() };
            DrawGameObjectNode(gameObject);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        {
            selectionContext = {};
        }

        ImGui::End();

        ImGui::Begin("Inspector");
        // Why? (in reference to the operator bool call)
        if (selectionContext.operator bool())
        {
            DrawComponents(selectionContext);
        }
        ImGui::End();
    }

    void GameObjectsPanel::DrawGameObjectNode(GameObject gameObject)
    {
        auto& name = context->registry.get<NameComponent>(gameObject).name;

        ImGuiTreeNodeFlags flags = ((selectionContext == gameObject) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)gameObject, flags, name.c_str());
        if (ImGui::IsItemClicked())
        {
            selectionContext = gameObject;
        }

        if (opened)
        {
            ImGui::TreePop();
        }
    }

    void GameObjectsPanel::DrawComponents(GameObject selection)
    {
        if (context == nullptr || selection.scene == nullptr)
        {
            return;
        }
        if (selection.HasComponent<NameComponent>())
        {
            auto& name = selection.GetComponent<NameComponent>().name;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), name.c_str());
            if (ImGui::InputText("Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer);
            }
        }

        if (selection.HasComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*) typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& transform = selection.GetComponent<TransformComponent>();
                ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.5f);
                ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.1f);
                ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);

                ImGui::TreePop();
            }
        }

        if (selection.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*) typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera Component"))
            {
                auto& cameraComponent = selection.GetComponent<CameraComponent>();

                const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
                const char* currentProjectionString = projectionTypeStrings[(int) cameraComponent.Camera.projectionType];

                if (ImGui::BeginCombo("Projection Type", currentProjectionString))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = currentProjectionString == projectionTypeStrings[i];
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                        {
                            currentProjectionString = projectionTypeStrings[i];
                            cameraComponent.Camera.projectionType = static_cast<SceneCamera::ProjectionType>(i);
                        }

                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo();
                }

                if (cameraComponent.Camera.projectionType == SceneCamera::ProjectionType::Orthographic)
                {
                    // TODO: Implement setting orthographic data that isn't based on the window viewport
                    float orthoNear = cameraComponent.Camera.GetNearClip();
                    if (ImGui::DragFloat("Near Clip", &orthoNear, 0.1f))
                        cameraComponent.Camera.SetNearClip(orthoNear);

                    float orthoFar = cameraComponent.Camera.GetFarClip();
                    if (ImGui::DragFloat("Far Clip", &orthoFar, 0.1f))
                        cameraComponent.Camera.SetFarClip(orthoFar);
                }
                if (cameraComponent.Camera.projectionType == SceneCamera::ProjectionType::Perspective)
                {
                    float verticalFOV = cameraComponent.Camera.perspectiveVerticalFOV;
                    if (ImGui::DragFloat("Vertical FOV", &verticalFOV, 0.1f))
                    {
                       cameraComponent.Camera.SetPerspectiveFOV(verticalFOV);
                    }
                    float perspectiveNear = cameraComponent.Camera.GetNearClip();
                    spdlog::info("{}", perspectiveNear);
                    if (ImGui::DragFloat("Near Clip", &perspectiveNear, 0.1f))
                        cameraComponent.Camera.SetNearClip(perspectiveNear);

                    float perspectiveFar = cameraComponent.Camera.GetFarClip();
                    if (ImGui::DragFloat("Far Clip", &perspectiveFar, 0.1f))
                        cameraComponent.Camera.SetFarClip(perspectiveFar);
                }

                ImGui::TreePop();
            }
        }
    }
}