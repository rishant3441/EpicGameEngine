#include "ExampleLayer.h"
#include <EpicGameEngine.h>

void ExampleLayer::OnAttach()
{
    EpicGameEngine::Renderer::enableDrawingToTexture = false;
    activeScene = std::make_shared<EpicGameEngine::Scene>();

    SDL_Color color;
    color.a = 255;
    color.r = 255;
    color.g = 0;
    color.b = 0;
    
    auto rect = activeScene->CreateGameObject("Rectangle");
    rect.AddComponent<EpicGameEngine::SpriteRendererComponent>(color);
    rect.GetComponent<EpicGameEngine::TransformComponent>().Position.x = 590;
    rect.GetComponent<EpicGameEngine::TransformComponent>().Position.y = 310;

    camera = activeScene->CreateGameObject("Camera");
    camera.AddComponent<EpicGameEngine::CameraComponent>();
    auto& cameraTransform = camera.GetComponent<EpicGameEngine::TransformComponent>();
    auto& cc = camera.GetComponent<EpicGameEngine::CameraComponent>();
    cameraTransform.Position.z = -1;
    camera.GetComponent<EpicGameEngine::CameraComponent>().Camera.SetViewportSize(1280, 720);
    using namespace EpicGameEngine;
    class CameraContrller : public ScriptableGameObject
    {

        void OnUpdate(Timestep time)
        {
            auto& cameraTransform = camera.GetComponent<TransformComponent>();

            if (Input::isKeyPressed(Keyboard::W))
            {
                cameraTransform.Position.y -= 300 * time.GetSeconds();
            }

            if (Input::isKeyPressed(Keyboard::A))
            {
                cameraTransform.Position.x -= 300 * time.GetSeconds();
            }

            if (Input::isKeyPressed(Keyboard::S))
            {
                cameraTransform.Position.y += 300 * time.GetSeconds();
            }

            if (Input::isKeyPressed(Keyboard::D))
            {
                cameraTransform.Position.x += 300 * time.GetSeconds();
            }
        }

        void OnStart()
        {

        }
    };
    camera.AddComponent<NativeScriptComponent>().Bind<CameraContrller>();
}

using namespace EpicGameEngine;

void ExampleLayer::OnRender()
{
}

void ExampleLayer::OnUpdate(Timestep time)
{
    activeScene->OnUpdate(time);
}