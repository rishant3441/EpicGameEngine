//
// Created by Rishan Thangaraj on 7/7/2022.
//

#include <EpicGameEngine/GameObjects/Scene.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/GameObjects/Components.h>
#include <EpicGameEngine/Renderer/Renderer.h>
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace EpicGameEngine
{
    GameObject Scene::CreateGameObject(const std::string &name)
    {
       GameObject gameObject =  { registry.create(), this };
       gameObject.AddComponent<TransformComponent>();
       gameObject.AddComponent<NameComponent>(name.empty() ? "Empty Game Object" : name);
       return gameObject;
    }

    void Scene::OnUpdate(Timestep ts)
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

        Camera* mainCamera = nullptr;
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

                    // TODO: Use "Primary" Camera Property to allow for multiple cameras
                    if (mainCamera != nullptr)
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
                            GPU_Ortho(mainCamera->GetLeft(), mainCamera->GetRight(), mainCamera->GetBottom(), mainCamera->GetTop(), mainCamera->orthographicNear, mainCamera->orthographicFar);
                            GPU_Translate(-cameraTransform->Position.x, cameraTransform->Position.y, cameraTransform->Position.z);
                            // TODO: Implement Rotation
                            GPU_Scale(cameraTransform->Scale.x, cameraTransform->Scale.y, 1);
                            GPU_MatrixCopy(GPU_GetProjection(), GPU_GetCurrentMatrix());
                            GPU_MatrixMode(Renderer::target, GPU_MODEL);
                        }
                        if (mainCamera->projectionType == SceneCamera::ProjectionType::Perspective)
                        {
                            GPU_SetActiveTarget(Renderer::target);
                            GPU_MatrixMode(Renderer::target, GPU_PROJECTION);
                            GPU_LoadIdentity();
                            //GPU_Perspective(mainCamera->perspectiveVerticalFOV, mainCamera->aspectRatio, mainCamera->perspectiveNear, mainCamera->perspectiveFar);
                            GPU_Frustum(mainCamera->GetLeft(), mainCamera->GetRight(), mainCamera->GetBottom(), mainCamera->GetTop(), mainCamera->perspectiveNear, mainCamera->perspectiveFar);
                            //float fH = tanf((mainCamera->perspectiveVerticalFOV / 360) * 3.14159265359) * mainCamera->perspectiveNear;
                            //float fW = fH * mainCamera->aspectRatio;
                            //spdlog::info("PH: {}, PW: {}, OH: {}, OW: {}", fH, fW, mainCamera->orthographicNear, mainCamera->orthographicFar);
                            GPU_Translate(-cameraTransform->Position.x, cameraTransform->Position.y, transform.Position.z);
                            // TODO: Implement Rotation
                            GPU_Scale(cameraTransform->Scale.x, cameraTransform->Scale.y, 1);
                            GPU_MatrixCopy(GPU_GetProjection(), GPU_GetCurrentMatrix());
                            GPU_MatrixMode(Renderer::target, GPU_MODEL);
                        }
                    }

                }
            }
        }

        auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto gameobject : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(gameobject);

            if(sprite.Texture != nullptr)
                Renderer::DrawTexturedRect(transform.Position.x, transform.Position.y, (float) transform.Scale.x * Renderer::unitSize, (float) transform.Scale.y * Renderer::unitSize, *sprite.Texture, 0, sprite.Color);
            else
                Renderer::DrawFilledRect(transform.Position.x, transform.Position.y, (float) transform.Scale.x * Renderer::unitSize, (float) transform.Scale.y * Renderer::unitSize, 0, sprite.Color);
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
}
