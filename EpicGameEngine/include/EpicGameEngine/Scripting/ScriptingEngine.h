//
// Created by Rishan Thangaraj on 7/15/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace EpicGameEngine
{
    class ScriptingEngine
    {
    public:
        static void Init();
        static void Shutdown();

    private:
        static void InitMono();
        static void ShutdownMono();

        static char* ReadBytes(const std::string& filepath, uint32_t* outSize);
        static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
        static void PrintAssemblyTypes(MonoAssembly* assembly);
        static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
        static MonoObject* InstantiateClass(const char* namespaceName, const char* className);
        static void CallMonoFunction(MonoObject* objectInstance, const char* name, unsigned int numberOfParams, void* params[]);

        uint8_t GetFieldAccessibility(MonoClassField *field);
        uint8_t GetPropertyAccessbility(MonoProperty *property);

        bool accessible(MonoClassField* field);
        bool accessible(MonoProperty* property);
        bool CheckMonoError(MonoError &error);
        std::string MonoStringToUTF8(MonoString *monoString);
    };
}

