//
// Created by Rishan Thangaraj on 7/15/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/Scripting/ScriptingEngine.h>
#include <EpicGameEngine/ege_pch.h>
#include <spdlog/spdlog.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>

namespace EpicGameEngine
{
    struct ScriptingEngineData
    {
        MonoDomain* RootDomain = nullptr;
        MonoDomain* AppDomain = nullptr;

        MonoAssembly* CoreAssembly = nullptr;
    };

    static ScriptingEngineData* data = nullptr;

    void EpicGameEngine::ScriptingEngine::Init()
    {
        data = new ScriptingEngineData();

        InitMono();
    }

    void EpicGameEngine::ScriptingEngine::Shutdown()
    {
        delete data;
    }

    void EpicGameEngine::ScriptingEngine::InitMono()
    {
        mono_set_assemblies_path("mono/lib");

        MonoDomain* rootDomain = mono_jit_init("EGEScriptingRuntime");
        if (rootDomain == nullptr)
        {
            spdlog::info("SCRIPTING ENGINE: LARGE ERROR OCCURRED");
            exit(1);
        }

        data->RootDomain = rootDomain;

        data->AppDomain = mono_domain_create_appdomain((char*) "EGEAppDomain", nullptr);
        mono_domain_set(data->AppDomain, true);

        MonoAssembly* assembly = LoadCSharpAssembly("EpicGameEngine-Scripting.dll");
        data->CoreAssembly = assembly;
    }

    void EpicGameEngine::ScriptingEngine::ShutdownMono()
    {

    }

    char *ScriptingEngine::ReadBytes(const std::string &filepath, uint32_t *outSize)
    {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
        if (!stream)
        {
            spdlog::warn("SCRIPTING ENGINE: Failed to open the file.");
            return nullptr;
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint32_t size = end - stream.tellg();


        if (size == 0)
        {
            spdlog::warn("SCRIPTING ENGINE: File is empty.");
            return nullptr;
        }

        char* buffer = new char[size];
        stream.read((char*) buffer, size);
        stream.close();

        *outSize = size;
        return buffer;
    }

    MonoAssembly *ScriptingEngine::LoadCSharpAssembly(const std::string &assemblyPath)
    {
        uint32_t fileSize = 0;
        char* fileData = ReadBytes(assemblyPath, &fileSize);

        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            const char* errorMessage = mono_image_strerror(status);
            spdlog::error("SCRIPTING ENGINE: Mono Image is not ok: {}", errorMessage);
            return nullptr;
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
        mono_image_close(image);

        delete[] fileData;

        return assembly;
    }

    void ScriptingEngine::PrintAssemblyTypes(MonoAssembly *assembly)
    {
        MonoImage* image = mono_assembly_get_image(assembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
            printf("%s.%s\n", nameSpace, name);
        }
    }

    MonoClass* ScriptingEngine::GetClassInAssembly(MonoAssembly *assembly, const char *namespaceName, const char *className)
    {
        MonoImage* image = mono_assembly_get_image(assembly);
        MonoClass* klass = mono_class_from_name(image, namespaceName, className);

        if (klass == nullptr)
        {
            spdlog::warn("SCRIPTING ENGINE: Class is empty");
            return nullptr;
        }

        return klass;
    }

    MonoObject* ScriptingEngine::InstantiateClass(const char *namespaceName, const char *className)
    {
        MonoClass* testingClass = GetClassInAssembly(data->CoreAssembly, namespaceName, className);
        MonoObject* classInstance = mono_object_new(data->AppDomain, testingClass);

        if (classInstance == nullptr)
        {
            spdlog::error("SCRIPTING ENGINE: Class is nullptr");
            exit(1);
        }

        mono_runtime_object_init(classInstance);
        return classInstance;
    }

    void ScriptingEngine::CallMonoFunction(MonoObject* objectInstance, const char* name, unsigned int numberOfParams, void* params[])
    {
        MonoClass* instanceClass = mono_object_get_class(objectInstance);
        MonoMethod* method = mono_class_get_method_from_name(instanceClass, name, numberOfParams);

        if (method == nullptr)
        {
            spdlog::warn("SCRIPTING ENGINE: No method called {} with {} parameter(s) in the class.", name, numberOfParams);
            return;
        }

        MonoObject* exception = nullptr;
        mono_runtime_invoke(method, objectInstance, params, &exception);
    }

    enum class Accessibility : uint8_t
    {
        None = 0,
        Private = (1 << 0),
        Internal = (1 << 1),
        Protected = (1 << 2),
        Public = (1 << 3)
    };

    uint8_t ScriptingEngine::GetFieldAccessibility(MonoClassField* field)
    {
        uint8_t accessibility = (uint8_t)Accessibility::None;
        uint32_t accessFlag = mono_field_get_flags(field) & MONO_FIELD_ATTR_FIELD_ACCESS_MASK;

        switch (accessFlag)
        {
            case MONO_FIELD_ATTR_PRIVATE:
            {
                accessibility = (uint8_t)Accessibility::Private;
                break;
            }
            case MONO_FIELD_ATTR_FAM_AND_ASSEM:
            {
                accessibility |= (uint8_t)Accessibility::Protected;
                accessibility |= (uint8_t)Accessibility::Internal;
                break;
            }
            case MONO_FIELD_ATTR_ASSEMBLY:
            {
                accessibility = (uint8_t)Accessibility::Internal;
                break;
            }
            case MONO_FIELD_ATTR_FAMILY:
            {
                accessibility = (uint8_t)Accessibility::Protected;
                break;
            }
            case MONO_FIELD_ATTR_FAM_OR_ASSEM:
            {
                accessibility |= (uint8_t)Accessibility::Private;
                accessibility |= (uint8_t)Accessibility::Protected;
                break;
            }
            case MONO_FIELD_ATTR_PUBLIC:
            {
                accessibility = (uint8_t)Accessibility::Public;
                break;
            }
        }

        return accessibility;
    }
    uint8_t ScriptingEngine::GetPropertyAccessbility(MonoProperty* property)
    {
        uint8_t accessibility = (uint8_t)Accessibility::None;

        // Get a reference to the property's getter method
        MonoMethod* propertyGetter = mono_property_get_get_method(property);
        if (propertyGetter != nullptr)
        {
            // Extract the access flags from the getters flags
            uint32_t accessFlag = mono_method_get_flags(propertyGetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;

            switch (accessFlag)
            {
                case MONO_FIELD_ATTR_PRIVATE:
                {
                    accessibility = (uint8_t)Accessibility::Private;
                    break;
                }
                case MONO_FIELD_ATTR_FAM_AND_ASSEM:
                {
                    accessibility |= (uint8_t)Accessibility::Protected;
                    accessibility |= (uint8_t)Accessibility::Internal;
                    break;
                }
                case MONO_FIELD_ATTR_ASSEMBLY:
                {
                    accessibility = (uint8_t)Accessibility::Internal;
                    break;
                }
                case MONO_FIELD_ATTR_FAMILY:
                {
                    accessibility = (uint8_t)Accessibility::Protected;
                    break;
                }
                case MONO_FIELD_ATTR_FAM_OR_ASSEM:
                {
                    accessibility |= (uint8_t)Accessibility::Private;
                    accessibility |= (uint8_t)Accessibility::Protected;
                    break;
                }
                case MONO_FIELD_ATTR_PUBLIC:
                {
                    accessibility = (uint8_t)Accessibility::Public;
                    break;
                }
            }
        }

        // Get a reference to the property's setter method
        MonoMethod* propertySetter = mono_property_get_set_method(property);
        if (propertySetter != nullptr)
        {
            // Extract the access flags from the setters flags
            uint32_t accessFlag = mono_method_get_flags(propertySetter, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
            if (accessFlag != MONO_FIELD_ATTR_PUBLIC)
                accessibility = (uint8_t)Accessibility::Private;
        }
        else
        {
            accessibility = (uint8_t)Accessibility::Private;
        }

        return accessibility;
    }

    bool ScriptingEngine::accessible(MonoClassField* field)
    {
        uint8_t accessibility = GetFieldAccessibility(field);
        if (accessibility & (uint8_t) Accessibility::Public) return true;
        return false;
    }

    bool ScriptingEngine::accessible(MonoProperty* property)
    {
        uint8_t accessibility = GetPropertyAccessbility(property);
        if (accessibility & (uint8_t) Accessibility::Public) return true;
        return false;
    }
    bool ScriptingEngine::CheckMonoError(MonoError& error)
    {
        bool hasError = !mono_error_ok(&error);
        if (hasError)
        {
            unsigned short errorCode = mono_error_get_error_code(&error);
            const char* errorMessage = mono_error_get_message(&error);
            spdlog::error("Mono Error!\n");
            spdlog::error("\tErrorCode: {}\n", errorCode);
            spdlog::error("\tError Message: {}\n", errorMessage);
            mono_error_cleanup(&error);
        }
        return hasError;
    }

    std::string ScriptingEngine::MonoStringToUTF8(MonoString* monoString)
    {
        if (monoString == nullptr || mono_string_length(monoString) == 0)
            return "";

        MonoError error;
        char* utf8 = mono_string_to_utf8_checked(monoString, &error);
        if (CheckMonoError(error))
            return "";
        std::string result(utf8);
        mono_free(utf8);
        return result;
    }
}

