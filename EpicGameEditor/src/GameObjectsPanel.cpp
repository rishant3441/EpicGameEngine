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
#include <imgui_internal.h>

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

    void GameObjectsPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
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
                DrawVec3Control("Position", transform.Position);
                DrawVec3Control("Rotation", transform.Rotation);
                DrawVec3Control("Scale", transform.Scale, 1.0f);

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

                ImGui::Checkbox("Primary Camera", &cameraComponent.Primary);

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
        if (selection.HasComponent<SpriteRendererComponent>())
        {
            if (ImGui::TreeNodeEx((void*) typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
            {
                auto& src = selection.GetComponent<SpriteRendererComponent>();

                // TODO: Make this cleaner (probably just remove some casts)
                float color[4] = { static_cast<float>((float) src.Color.r / 255.0f), static_cast<float>((float) src.Color.g / 255.0f), static_cast<float>((float) src.Color.b / 255.0f), static_cast<float>((float) src.Color.a / 255.0f)};
                ImGui::ColorEdit4("Color", color);

                src.Color.r = color[0] * 255.0;
                src.Color.g = color[1] * 255.0;
                src.Color.b = color[2] * 255.0;
                src.Color.a = color[3] * 255.0;

                ImGui::TreePop();
            }
        }
    }
}