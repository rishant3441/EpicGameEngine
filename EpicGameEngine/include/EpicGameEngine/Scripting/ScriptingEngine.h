//
// Created by Rishan Thangaraj on 7/15/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once

#include <EpicGameEngine/GameObjects/Scene.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace EpicGameEngine
{
    class ScriptClass
    {
    public:
        ScriptClass() = default;
        ScriptClass(const std::string& classNamespace, const std::string& className);

        MonoObject* InstantiateClass(); /// \brief Only instantiates the class
        MonoMethod* GetMethod(const std::string& name, int numOfParameters);
        MonoObject* CallMethod(MonoObject* object, MonoMethod* method, void** params = nullptr);

    private:
        std::string classNamespace;
        std::string className;

        MonoClass* monoClass = nullptr;
    };

    class ScriptInstance
    {
    public:
        ScriptInstance(Ref<ScriptClass> scriptClass);

        void CallOnStart();
        void CallOnUpdate(float ts);
    private:
        Ref<ScriptClass> scriptClass;

                MonoObject* instance = nullptr;
                MonoMethod* onStartMethod = nullptr;
                MonoMethod* onUpdateMethod = nullptr;
            };
    class ScriptingEngine
    {
    public:
        static void Init();
        static void Shutdown();

        static std::unordered_map<std::string, Ref<ScriptClass>> GetGameObjectClasses();

        friend class ScriptClass;

        static void OnRuntimeStart(Scene* scene);
        static void OnRuntimeStop();

        static bool ClassExists(const std::string& fullName);

        static void OnGOCreate(GameObject gameObject);

    private:
        static void InitMono();
        static void ShutdownMono();

        // TODO: Clean this up (via seperate namespaces, classes, etc.)
        static char* ReadBytes(const std::string& filepath, uint32_t* outSize);
        static MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
        static void PrintAssemblyTypes(MonoAssembly* assembly);
        static MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* namespaceName, const char* className);
        static MonoObject* InstantiateClass(const char* namespaceName, const char* className);
        static MonoObject* InstantiateClass(MonoClass* monoClass);
        static void CallMonoFunction(MonoObject* objectInstance, const char* name, unsigned int numberOfParams, void* params[] = nullptr);

        uint8_t GetFieldAccessibility(MonoClassField *field);
        uint8_t GetPropertyAccessbility(MonoProperty *property);

        bool accessible(MonoClassField* field);
        bool accessible(MonoProperty* property);
        bool CheckMonoError(MonoError &error);
        std::string MonoStringToUTF8(MonoString *monoString);

        static void LoadAssemblyClasses(MonoAssembly *pAssembly);
    };
}

