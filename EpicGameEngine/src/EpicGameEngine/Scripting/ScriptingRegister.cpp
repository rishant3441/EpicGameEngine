//
// Created by Rishan Thangaraj on 7/19/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/Scripting/ScriptingRegister.h>
#include <EpicGameEngine/UUID.h>
#include <EpicGameEngine/GameObjects/Scene.h>
#include <EpicGameEngine/Scripting/ScriptingEngine.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/GameObjects/Components.h>
#include <EpicGameEngine/Input/Input.h>
#include <mono/metadata/object.h>

namespace EpicGameEngine
{
#define EGE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("EpicGameEngine.InternalCalls::" #Name, Name)

    static void TransformC_GetPosition(UUID gameObjectID, glm::vec3* outPosition)
    {
        Scene* scene = ScriptingEngine::GetCurrentScene();
        GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);

        *outPosition = gameObject.GetComponent<TransformComponent>().Position;
    }

    static void TransformC_SetPosition(UUID gameObjectID, glm::vec3* position)
    {
        Scene* scene = ScriptingEngine::GetCurrentScene();
        GameObject gameObject = scene->GetGameObjectByUUID(gameObjectID);

        gameObject.GetComponent<TransformComponent>().Position = *position;
    }

    static bool Input_IsKeyDown(KeyCode keyCode)
    {
        if (Input::isKeyPressed(keyCode))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    static bool Input_IsMouseDown(MouseCode mouseCode)
    {
        if (Input::isMouseButtonPressed(mouseCode))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void ScriptingRegister::RegisterScripts()
    {
        // TRANSFORM COMPONENTS
        EGE_ADD_INTERNAL_CALL(TransformC_GetPosition);
        EGE_ADD_INTERNAL_CALL(TransformC_SetPosition);
        // INPUT
        EGE_ADD_INTERNAL_CALL(Input_IsKeyDown);
        EGE_ADD_INTERNAL_CALL(Input_IsMouseDown);
    }
}
