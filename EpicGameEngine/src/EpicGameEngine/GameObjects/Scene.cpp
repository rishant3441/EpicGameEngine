//
// Created by Rishan Thangaraj on 7/7/2022.
//

#include <EpicGameEngine/GameObjects/Scene.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/GameObjects/Components.h>

namespace EpicGameEngine
{
    GameObject Scene::CreateGameObject(const std::string &name)
    {
       GameObject gameObject( registry.create(), this );
       gameObject.AddComponent<TransformComponent>();
       gameObject.AddComponent<NameComponent>(name.empty() ? "Empty Game Object" : name);
       return gameObject;
    }
}
