//
// Created by Rishan Thangaraj on 7/7/2022.
//

#include <EpicGameEngine/GameObjects/Scene.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/GameObjects/Components.h>
#include <EpicGameEngine/Renderer/Renderer.h>

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
}
