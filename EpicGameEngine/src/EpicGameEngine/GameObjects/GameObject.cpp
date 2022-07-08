//
// Created by Rishan Thangaraj on 7/7/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/GameObjects/Scene.h>

namespace EpicGameEngine
{
    GameObject::GameObject(entt::entity handle, Scene* scene)
        : entityHandle(handle), scene(scene)
    {

    }
}