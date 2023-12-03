//
// Created by Rishan Thangaraj on 7/7/2022.
//

#include <EpicGameEngine/GameObjects/Scene.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/GameObjects/Components.h>
#include <EpicGameEngine/Renderer/Renderer.h>
#include <EpicGameEngine/Renderer/Renderer2D.h>
#include <EpicGameEngine/Scripting/ScriptingEngine.h>
#include <EpicGameEngine/Application.h>
#include <EpicGameEngine/UUID.h>
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <EpicGameEngine/Debug.h>

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
            for_each(std::execution::par, view.begin(), view.end(), [&](auto gameObject){
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(gameObject);

                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = &transform;
                }
            });
        }

        Renderer2D::BeginScene(*mainCamera, cameraTransform->GetTransform());
        // TODO: Clean this up - probably put into DrawRect function
        // TODO: NEXT TIME TRY A VIEW TO MAKE MULTITHREADINGGGGGGGGG
        {
            auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            std::mutex mtx;
            for_each(std::execution::par, group.begin(), group.end(), [&](auto gameobject){
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(gameobject);
                    Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
                });
        }

        {
            auto view = registry.view<TransformComponent, LightEmitterComponent>();
            for_each(std::execution::par, view.begin(), view.end(), [&](auto gameobject){
                auto& transform = view.get<TransformComponent>(gameobject);
                auto& lec = view.get<LightEmitterComponent>(gameobject);
                // Application::Get().lightingSystem->RenderAt(transform.Position, lec.lightRadius);
            });
        }
        Renderer2D::EndScene();
    }

    void Scene::OnEditorUpdate(Timestep ts, EditorCamera& camera)
    {
        Renderer2D::BeginScene(camera);
        // TODO: Clean this up - probably put into DrawRect function
        auto group = registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto gameobject : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(gameobject);

            Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
        }

        auto view = registry.view<TransformComponent, LightEmitterComponent>();
        for_each(std::execution::par, view.begin(), view.end(), [&](auto gameobject){
            auto& transform = view.get<TransformComponent>(gameobject);
            auto& lec = view.get<LightEmitterComponent>(gameobject);
            //Application::Get().lightingSystem->RenderAt(transform.Position, lec.lightRadius);
        });
        Renderer2D::EndScene();
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

