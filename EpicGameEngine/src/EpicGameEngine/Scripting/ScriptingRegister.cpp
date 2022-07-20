//
// Created by Rishan Thangaraj on 7/19/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/Scripting/ScriptingRegister.h>
#include <mono/metadata/object.h>

namespace EpicGameEngine
{
#define EGE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("EpicGameEngine.InternalCalls::" #Name, Name)

    void ScriptingRegister::RegisterScripts()
    {

    }
}
