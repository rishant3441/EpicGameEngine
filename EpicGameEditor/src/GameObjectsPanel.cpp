//
// Created by Rishan Thangaraj on 7/21/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "GameObjectsPanel.h"
#include <EpicGameEngine.h>
#include <EpicGameEngine/Scripting/ScriptingEngine.h>
#include <imgui.h>
#include <map>
#include <unordered_map>

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

        // blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty GameObject"))
            {
                context->CreateGameObject("Empty Game Object");
            }

            ImGui::EndPopup();
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
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)gameObject, flags, name.c_str());
        if (ImGui::IsItemClicked())
        {
            selectionContext = gameObject;
        }

        bool gameObjectDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Game Object"))
            {
                gameObjectDeleted = true;
            }

            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGui::TreePop();
        }

        if (gameObjectDeleted)
        {
            context->DeleteGameObject(gameObject);
            if (selectionContext == gameObject)
                selectionContext = {};
        }
    }

    template <typename componentType, typename UIFunction>
    static void DrawComponent(const std::string& name, GameObject gameObject, UIFunction uiFunction)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (gameObject.HasComponent<componentType>())
        {
            auto& component = gameObject.GetComponent<componentType>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(componentType).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open)
            {
                uiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                gameObject.RemoveComponent<componentType>();
        }
    }

    void GameObjectsPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];
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
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopFont();
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
            if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
            {
                name = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponent");
        }

        if (ImGui::BeginPopup("AddComponent"))
        {
            // TODO: Clean this up (for ex. move into seperate function or something)
            if (ImGui::MenuItem("Sprite Renderer"))
            {
                selectionContext.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Camera"))
            {
                selectionContext.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("CSharp Script"))
            {
                selectionContext.AddComponent<CSharpScriptComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Light Emitter"))
            {
                selectionContext.AddComponent<LightEmitterComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        DrawComponent<TransformComponent>("Transform", selection, [&](auto& component)
        {
            DrawVec3Control("Position", component.Position);
            //glm::vec3 rotation = { -glm::degrees(component.Rotation.x), glm::degrees(component.Rotation.y), component.Rotation.z };
            glm::vec3 rotation = glm::degrees(component.Rotation);
            DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", component.Scale, 1.0f);
        });

        DrawComponent<CameraComponent>("Camera Component", selection, [](auto& component)
        {

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
            const char* currentProjectionString = projectionTypeStrings[(int) component.Camera.projectionType];

            if (ImGui::BeginCombo("Projection Type", currentProjectionString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                    {
                        currentProjectionString = projectionTypeStrings[i];
                        component.Camera.projectionType = static_cast<SceneCamera::ProjectionType>(i);
                    }

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::Checkbox("Primary Camera", &component.Primary);

            if (component.Camera.projectionType == SceneCamera::ProjectionType::Orthographic)
            {
                // TODO: Implement setting orthographic data that isn't based on the window viewport
                float orthoNear = component.Camera.GetNearClip();
                if (ImGui::DragFloat("Near Clip", &orthoNear, 0.1f))
                    component.Camera.SetNearClip(orthoNear);

                float orthoFar = component.Camera.GetFarClip();
                if (ImGui::DragFloat("Far Clip", &orthoFar, 0.1f))
                    component.Camera.SetFarClip(orthoFar);
            }
            if (component.Camera.projectionType == SceneCamera::ProjectionType::Perspective)
            {
                float verticalFOV = component.Camera.perspectiveVerticalFOV;
                if (ImGui::DragFloat("Vertical FOV", &verticalFOV, 0.1f))
                {
                    component.Camera.SetPerspectiveFOV(verticalFOV);
                }
                float perspectiveNear = component.Camera.GetNearClip();
                if (ImGui::DragFloat("Near Clip", &perspectiveNear, 0.1f))
                    component.Camera.SetNearClip(perspectiveNear);

                float perspectiveFar = component.Camera.GetFarClip();
                if (ImGui::DragFloat("Far Clip", &perspectiveFar, 0.1f))
                    component.Camera.SetFarClip(perspectiveFar);
            }
        });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", selection, [](auto& component)
        {
            float color[4] = { static_cast<float>((float) component.Color.r / 255.0f), static_cast<float>((float) component.Color.g / 255.0f), static_cast<float>((float) component.Color.b / 255.0f), static_cast<float>((float) component.Color.a / 255.0f)};
            ImGui::ColorEdit4("Color", color);

            component.Color.r = color[0] * 255.0;
            component.Color.g = color[1] * 255.0;
            component.Color.b = color[2] * 255.0;
            component.Color.a = color[3] * 255.0;
        });

        DrawComponent<CSharpScriptComponent>("CSharp Script", selection, [](auto& component) {
            bool scriptClassExists = ScriptingEngine::ClassExists(component.name);

            static char buffer[64];
            strcpy_s(buffer, sizeof(buffer), component.name.c_str());

            if (!scriptClassExists)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

            if (ImGui::InputText("Class Name", buffer, sizeof(buffer)))
            {
                // Game Object Classes
                component.name = buffer;
            }

            if (!scriptClassExists)
                ImGui::PopStyleColor();
        });

        DrawComponent<LightEmitterComponent>("Light Emitter", selection, [](auto& component)
        {
            ImGui::DragFloat("Radius", &component.lightRadius);
        });
    }
}