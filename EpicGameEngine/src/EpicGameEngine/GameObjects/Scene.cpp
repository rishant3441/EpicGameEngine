//
// Created by Rishan Thangaraj on 7/7/2022.
//

#include <EpicGameEngine/GameObjects/Scene.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/GameObjects/Components.h>
#include <EpicGameEngine/Renderer/Renderer.h>
#include <EpicGameEngine/Scripting/ScriptingEngine.h>
#include <EpicGameEngine/UUID.h>
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace EpicGameEngine
{
    GameObject Scene::CreateGameObject(const std::string &name)
    {
        return CreateGameObjectWithUUID(name, UUID());
    }

    GameObject Scene::CreateGameObjectWithUUID(const std::string &name, UUID uuid)
    {
        GameObject gameObject =  { registry.create(), this };
        gameObject.AddComponent<IDComponent>(uuid);
        gameObject.AddComponent<TransformComponent>();
        gameObject.AddComponent<NameComponent>(name.empty() ? "Empty Game Object" : name);

        entityMap[uuid] = gameObject; 

        return gameObject;
    }

    void Scene::DeleteGameObject(GameObject gameObject)
    {
        registry.destroy(gameObject);
        entityMap.erase(gameObject.GetUUID());
    }

    void Scene::OnRuntimeUpdate(Timestep ts)
    {
        registry.view<NativeScriptComponent>().each([=](auto gameObject, auto& script)
        {
            if (!script.Instance)
            {
                script.Instance = script.InstantiateScript();
                script.Instance->gameObject = {gameObject, this };
                script.Instance->OnStart();
            }

            script.Instance->OnUpdate(ts);
        });

        auto view = registry.view<CSharpScriptComponent>();
        for (auto gameObjectID : view)
        {
            GameObject gameObject = { gameObjectID, this };
            ScriptingEngine::OnGOUpdate(gameObject, ts.GetSeconds());
        }

        SceneCamera* mainCamera = nullptr;
        TransformComponent* cameraTransform;
        {
            auto view = registry.view<TransformComponent, CameraComponent>();
            for (auto gameObject : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(gameObject);

                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = &transform;

                    if (mainCamera != nullptr && camera.Primary == true)
                    {
                        /*GPU_MatrixMode(Renderer::target, GPU_PROJECTION);
                        GPU_LoadIdentity();
                        GPU_Ortho(mainCamera->GetLeft(), mainCamera->GetRight(), mainCamera->GetBottom(), mainCamera->GetTop(), mainCamera->orthographicNear, mainCamera->orthographicFar);
                        GPU_MatrixMode(Renderer::target, GPU_MODEL);
                        GPU_SetCamera(Renderer::target, nullptr);*/
                        if (mainCamera->projectionType == SceneCamera::ProjectionType::Orthographic)
                        {
                            GPU_SetActiveTarget(Renderer::target);
                            GPU_MatrixMode(Renderer::target, GPU_PROJECTION);
                            GPU_LoadIdentity();
                            GPU_Ortho(mainCamera->GetLeft(), mainCamera->GetRight(), mainCamera->GetTop(), mainCamera->GetBottom(), mainCamera->orthographicNear, mainCamera->orthographicFar);
                            GPU_Translate(-cameraTransform->Position.x, cameraTransform->Position.y, cameraTransform->Position.z);
                            // TODO: Implement Rotation
                            GPU_Scale(cameraTransform->Scale.x, cameraTransform->Scale.y, 1);
                            GPU_MatrixCopy(GPU_GetProjection(), GPU_GetCurrentMatrix());
                            GPU_MatrixMode(Renderer::target, GPU_MODEL);
                        }
                        if (mainCamera->projectionType == SceneCamera::ProjectionType::Perspective)
                        {
                            Renderer::target->use_camera = false;
                            glm::mat4 viewMatrix;
                            glm::mat4 projectionMatrix;

                            viewMatrix = cameraTransform->GetTransform();
                            viewMatrix = glm::inverse(viewMatrix);
                            projectionMatrix = glm::perspective(glm::radians(mainCamera->perspectiveVerticalFOV), mainCamera->aspectRatio, mainCamera->perspectiveNear, mainCamera->perspectiveFar);
                            GPU_SetActiveTarget(Renderer::target);
                            GPU_MatrixMode(Renderer::target, GPU_PROJECTION);
                            GPU_LoadIdentity();
                            GPU_MatrixCopy(GPU_GetProjection(), glm::value_ptr(projectionMatrix));
                            GPU_MatrixMode(Renderer::target, GPU_VIEW);
                            GPU_LoadIdentity();
                            GPU_MatrixCopy(GPU_GetView(), glm::value_ptr(viewMatrix));
                            GPU_MatrixMode(Renderer::target, GPU_MODEL);
                        }
                    }

                }
            }
        }

        // TODO: Clean this up - probably put into DrawRect function
        auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto gameobject : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(gameobject);

            if(sprite.Texture != nullptr)
            {
                Renderer::DrawTexturedRect(transform.Position.x, transform.Position.y, (float) transform.Scale.x * Renderer::unitSize, (float) transform.Scale.y * Renderer::unitSize, *sprite.Texture, 0, sprite.Color);
            }
            else
            {
                Renderer::unitSize = 1.0f;
                GPU_SetActiveTarget(Renderer::target);
                GPU_MatrixMode(Renderer::target, GPU_MODEL);
                GPU_LoadIdentity();
                float xCenter, yCenter;
                xCenter = transform.Position.x + (transform.Scale.x * Renderer::unitSize / 2);
                yCenter = transform.Position.y + (transform.Scale.y * Renderer::unitSize / 2);
                GPU_Translate(xCenter, yCenter, 0);
                GPU_Translate(0, 0, transform.Position.z);
                GPU_MultiplyAndAssign(GPU_GetCurrentMatrix(), glm::value_ptr(transform.GetRotation()));
                GPU_Scale(1, 1, Renderer::unitSize * transform.Scale.z);
                GPU_Translate(-xCenter, -yCenter, 0);
                GPU_MatrixCopy(GPU_GetModel(), GPU_GetCurrentMatrix());
                Renderer::DrawFilledRect(transform.Position.x, -transform.Position.y, (float) transform.Scale.x * Renderer::unitSize, (float) transform.Scale.y * Renderer::unitSize, 0, sprite.Color);
            }
        }
        
    }

    void Scene::OnEditorUpdate(Timestep ts, EditorCamera& camera)
    {
        GPU_SetActiveTarget(Renderer::target);
        GPU_SetCamera(Renderer::target, nullptr);
        Renderer::target->use_camera = false;
        GPU_MatrixMode(Renderer::target, GPU_PROJECTION);
        GPU_LoadIdentity();
        GPU_MatrixCopy(GPU_GetProjection(), glm::value_ptr(camera.GetProjectionMatrix()));
        GPU_MatrixMode(Renderer::target, GPU_VIEW);
        GPU_LoadIdentity();
        GPU_MatrixCopy(GPU_GetView(), glm::value_ptr(camera.GetViewMatrix()));
        GPU_MatrixMode(Renderer::target, GPU_MODEL);

        // TODO: Clean this up - probably put into DrawRect function
        auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto gameobject : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(gameobject);

            if(sprite.Texture != nullptr)
            {
                Renderer::DrawTexturedRect(transform.Position.x, transform.Position.y, (float) transform.Scale.x * Renderer::unitSize, (float) transform.Scale.y * Renderer::unitSize, *sprite.Texture, 0, sprite.Color);
            }
            else
            {
                Renderer::unitSize = 1.0f;
                GPU_SetActiveTarget(Renderer::target);
                GPU_MatrixMode(Renderer::target, GPU_MODEL);
                GPU_LoadIdentity();
                float xCenter, yCenter;
                xCenter = transform.Position.x + (transform.Scale.x * Renderer::unitSize / 2);
                yCenter = transform.Position.y + (transform.Scale.y * Renderer::unitSize / 2);
                GPU_Translate(xCenter, yCenter, 0);
                GPU_Translate(0, 0, transform.Position.z);
                GPU_MultiplyAndAssign(GPU_GetCurrentMatrix(), glm::value_ptr(transform.GetRotation()));
                GPU_Scale(1, 1, Renderer::unitSize * transform.Scale.z);
                GPU_Translate(-xCenter, -yCenter, 0);
                GPU_MatrixCopy(GPU_GetModel(), GPU_GetCurrentMatrix());
                Renderer::DrawFilledRect(transform.Position.x, -transform.Position.y, (float) transform.Scale.x * Renderer::unitSize, (float) transform.Scale.y * Renderer::unitSize, 0, sprite.Color);
            }
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        viewportSize.x = width;
        viewportSize.y = height;

        auto view = registry.view<CameraComponent>();
        for (auto gameObject : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(gameObject);
            if (!cameraComponent.fixedAspectRatio)
            {
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    }

    GameObject Scene::GetPrimaryCamera()
    {
        auto view = registry.view<CameraComponent>();
        for (auto gameObject : view)
        {
           const auto& camera = view.get<CameraComponent>(gameObject);
           if (camera.Primary)
               return GameObject{ gameObject, this };
        }
        return {};
    }

    void Scene::OnRuntimeStart()
    {
        ScriptingEngine::OnRuntimeStart(this);

        auto view = registry.view<CSharpScriptComponent>();
        for (auto gameObjectID : view)
        {
            GameObject gameObject = { gameObjectID, this };
            ScriptingEngine::OnGOCreate(gameObject);
        }
    }
    void Scene::OnRuntimeStop()
    {
        ScriptingEngine::OnRuntimeStop();
    }

    GameObject Scene::GetGameObjectByUUID(UUID uuid)
    {
        if (entityMap.find(uuid) != entityMap.end())
            return { entityMap.at(uuid), this };

        Debug::Log::LogWarn("SCENE: No gameObject with specified UUID {} was found in the scene.", uuid); 
        return {};
    }
}

