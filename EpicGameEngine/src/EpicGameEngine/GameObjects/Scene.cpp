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

    void Scene::DeleteGameObject(GameObject gameObject)
    {
        registry.destroy(gameObject);
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
                            float offsetX, offsetY;

                            GPU_SetActiveTarget(Renderer::target);
                            GPU_MatrixMode(Renderer::target, GPU_PROJECTION);
                            GPU_LoadIdentity();
                            //GPU_Perspective(mainCamera->perspectiveVerticalFOV, mainCamera->aspectRatio, mainCamera->perspectiveNear, mainCamera->perspectiveFar);
                            GPU_Frustum(mainCamera->GetLeft(), mainCamera->GetRight(), mainCamera->GetBottom(), mainCamera->GetTop(), mainCamera->perspectiveNear, mainCamera->perspectiveFar);
                            //float fH = tanf((mainCamera->perspectiveVerticalFOV / 360) * 3.14159265359) * mainCamera->perspectiveNear;
                            //float fW = fH * mainCamera->aspectRatio;
                            //spdlog::info("PH: {}, PW: {}, OH: {}, OW: {}", fH, fW, mainCamera->orthographicNear, mainCamera->orthographicFar);
                            GPU_Translate(-cameraTransform->Position.x, cameraTransform->Position.y, transform.Position.z);

                            offsetX = Renderer::target->w/2.0f;
                            offsetY = Renderer::target->h/2.0f;
                            GPU_Translate(offsetX, offsetY, 0);

                            // TODO: Implement Rotation
                            glm::mat4 rotation = glm::toMat4(glm::quat(transform.Rotation));
                            //GPU_MultiplyAndAssign(GPU_GetCurrentMatrix(), glm::value_ptr(rotation));
                            GPU_Rotate(transform.Rotation.x, 1, 0, 0);
                            GPU_Rotate(transform.Rotation.y, 0, 1, 0);
                            GPU_Rotate(transform.Rotation.z, 0, 0, 1);
                            GPU_Scale(cameraTransform->Scale.x, cameraTransform->Scale.y, cameraTransform->Scale.z);

                            GPU_Translate(-offsetX, -offsetY, 0);

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

            // TODO: Setup rotation
            if(sprite.Texture != nullptr)
            {
                Renderer::DrawTexturedRect(transform.Position.x, transform.Position.y, (float) transform.Scale.x * Renderer::unitSize, (float) transform.Scale.y * Renderer::unitSize, *sprite.Texture, 0, sprite.Color);
            }
            else
            {
                GPU_SetActiveTarget(Renderer::target);
                GPU_MatrixMode(Renderer::target, GPU_MODEL);
                GPU_LoadIdentity();
                float xCenter, yCenter;
                xCenter = transform.Position.x + (transform.Scale.x * 50 / 2);
                yCenter = transform.Position.y + (transform.Scale.y * 50 / 2);
                GPU_Translate(xCenter, yCenter, 0);
                GPU_Translate(0, 0, transform.Position.z);
                GPU_Rotate(transform.Rotation.x, 1.0f, 0, 0);
                GPU_Rotate(transform.Rotation.y, 0, 1.0f, 0);
                GPU_Rotate(-transform.Rotation.z, 0, 0, 1.0f);
                GPU_Scale(1, 1, 50 * transform.Scale.z);
                GPU_Translate(-xCenter, -yCenter, 0);
                Renderer::DrawFilledRect(transform.Position.x + (transform.Scale.x * 50 / 2), transform.Position.y + (transform.Scale.y * 50 / 2), (float) transform.Scale.x * Renderer::unitSize, (float) transform.Scale.y * Renderer::unitSize, 0, sprite.Color);
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
}
