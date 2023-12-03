//
// Created by Rishan Thangaraj on 7/31/2022.
//
// Copyright (c) 2022, Rishan Thangaraj <rishanthangaraj@gmail.com> All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <EpicGameEngine/GameObjects/SceneSerializer.h>

#include <EpicGameEngine/ege_pch.h>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>
#include <EpicGameEngine/GameObjects/GameObject.h>
#include <EpicGameEngine/GameObjects/Components.h>
#include <glm/glm.hpp>

namespace YAML
{
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template<>
    struct convert<SDL_Color>
    {
        static Node encode(const SDL_Color& rhs)
        {
            Node node;
            node.push_back(rhs.r);
            node.push_back(rhs.g);
            node.push_back(rhs.b);
            node.push_back(rhs.a);
            return node;
        }

        static bool decode(const Node& node, SDL_Color& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.r = node[0].as<uint8_t>();
            rhs.g = node[1].as<uint8_t>();
            rhs.b = node[2].as<uint8_t>();
            rhs.a = node[3].as<uint8_t>();
            return true;
        }
    };
}

namespace EpicGameEngine
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const SDL_Color& color)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << (uint32_t) color.r << (uint32_t) color.g << (uint32_t) color.b << (uint32_t) color.a << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(const Ref <Scene> &scene)
        : scene(scene)
    {
    }

    static void SerializeGameObject(YAML::Emitter& out, GameObject gameObject)
    {
       out << YAML::BeginMap;
       out << YAML::Key << "GameObject";
       out << YAML::Value << gameObject.GetUUID(); // TODO: Game Object ID

       if (gameObject.HasComponent<NameComponent>())
       {
           out << YAML::Key << "NameComponent";
           out << YAML::BeginMap;

           auto& name = gameObject.GetComponent<NameComponent>().name;
           out << YAML::Key << "Name" << YAML::Value << name;

           out << YAML::EndMap;
       }

       if (gameObject.HasComponent<TransformComponent>())
       {
           out << YAML::Key << "TransformComponent";
           out << YAML::BeginMap;

           auto& tc = gameObject.GetComponent<TransformComponent>();
           out << YAML::Key << "Position" << YAML::Value << tc.Position;
           out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
           out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

           out << YAML::EndMap; // TransformComponent
       }

       if (gameObject.HasComponent<CameraComponent>())
       {
           out << YAML::Key << "CameraComponent";
           out << YAML::BeginMap; // CameraComponent

           auto& cameraComponent = gameObject.GetComponent<CameraComponent>();
           auto& camera = cameraComponent.Camera;

           out << YAML::Key << "Camera" << YAML::Value;
           out << YAML::BeginMap; // Camera
           out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.projectionType;
           out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.perspectiveVerticalFOV;
           out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.perspectiveNear;
           out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.perspectiveFar;
           out << YAML::Key << "OrthographicNear" << YAML::Value << camera.orthographicNear;
           out << YAML::Key << "OrthographicFar" << YAML::Value << camera.orthographicFar;
           out << YAML::EndMap; // Camera

           out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
           out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

           out << YAML::EndMap; // CameraComponent
       }

       if (gameObject.HasComponent<SpriteRendererComponent>())
       {
           out << YAML::Key << "SpriteRendererComponent";
           out << YAML::BeginMap; // SpriteRendererComponent

           auto& spriteRendererComponent = gameObject.GetComponent<SpriteRendererComponent>();
           out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

           out << YAML::EndMap; // SpriteRendererComponent
       }

       if (gameObject.HasComponent<CSharpScriptComponent>())
       {
           auto& scriptComponent = gameObject.GetComponent<CSharpScriptComponent>();

           out << YAML::Key << "CSharpScriptComponent";
           out << YAML::BeginMap;
           out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.name;
           out << YAML::EndMap;
       }

       out << YAML::EndMap;
    }

    void SceneSerializer::Serialize(const std::string &filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << "Unnamed Scene";
        out << YAML::Key << "GameObjects";
        out << YAML::Value << YAML::BeginSeq;
        scene->registry.each([&](auto gameObjectID)
         {
             GameObject gameObject = { gameObjectID, scene.get() };

             if (gameObject.operator unsigned int() == entt::null)
             {
                return;
             }

             SerializeGameObject(out, gameObject);
         });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string &filepath)
    {
        spdlog::error("SerializeRuntime not implemented");
        exit(-1);
    }

    bool SceneSerializer::Deserialize(const std::string &filepath)
    {
        std::ifstream stream(filepath);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();
        spdlog::info("ENGINE: Deserializing scene '{0}'", sceneName);

        auto gameObjects = data["GameObjects"];
        if (gameObjects)
        {
            for (auto gameObject : gameObjects)
            {
                uint64_t uuid = gameObject["GameObject"].as<uint64_t>();

                std::string name;
                auto nameComponent = gameObject["NameComponent"];
                if (nameComponent)
                    name = nameComponent["Name"].as<std::string>();

                spdlog::info("ENGINE: Deserialized gameObject with ID = {0}, name = {1}", uuid, name);

                GameObject deserializedGameObject = scene->CreateGameObjectWithUUID(name, uuid);

                auto transformComponent = gameObject["TransformComponent"];
                if (transformComponent)
                {
                    // GameObjects always have transforms
                    auto& tc = deserializedGameObject.GetComponent<TransformComponent>();
                    tc.Position = transformComponent["Position"].as<glm::vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                }

                auto cameraComponent = gameObject["CameraComponent"];
                if (cameraComponent)
                {
                    auto& cc = deserializedGameObject.AddComponent<CameraComponent>();

                    auto cameraProps = cameraComponent["Camera"];
                    cc.Camera.projectionType = (SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>();

                    cc.Camera.perspectiveVerticalFOV = cameraProps["PerspectiveFOV"].as<float>();
                    cc.Camera.perspectiveNear = cameraProps["PerspectiveNear"].as<float>();
                    cc.Camera.perspectiveFar = cameraProps["PerspectiveFar"].as<float>();

                    cc.Camera.orthographicNear = cameraProps["OrthographicNear"].as<float>();
                    cc.Camera.orthographicFar = cameraProps["OrthographicFar"].as<float>();

                    cc.Primary = cameraComponent["Primary"].as<bool>();
                    cc.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

                    cc.Camera.SetViewportSize(850, 536); // TODO: Don't hardcode the starter window viewport
                }

                auto cSharpScriptComponent = gameObject["CSharpScriptComponent"];
                if (cSharpScriptComponent)
                {
                    auto& sc = deserializedGameObject.AddComponent<CSharpScriptComponent>();
                    sc.name = cSharpScriptComponent["ClassName"].as<std::string>();
                }

                auto spriteRendererComponent = gameObject["SpriteRendererComponent"];
                if (spriteRendererComponent)
                {
                    auto& src = deserializedGameObject.AddComponent<SpriteRendererComponent>();
                    src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
                }
            }
        }

        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string &filepath)
    {
        spdlog::error("DeserializeRuntime not implemented");
        exit(-1);
    }
}