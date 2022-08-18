//
// Created by Rishan Thangaraj on 7/7/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/GameObjects/Scene.h>
#include <EpicGameEngine/GameObjects/Components.h>

namespace EpicGameEngine
{
    GameObject::GameObject(entt::entity handle, Scene* newScene)

            : entityHandle(handle), scene(newScene)
    {

    }

    UUID GameObject::GetUUID()
    {
       return GetComponent<IDComponent>().ID;
    }
}