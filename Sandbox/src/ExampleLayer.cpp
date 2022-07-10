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

    EpicGameEngine::CameraController::CreateCamera();
}

using namespace EpicGameEngine;

void ExampleLayer::OnRender()
{
    activeScene->OnUpdate();
}

void ExampleLayer::OnUpdate(Timestep time)
{
    if (Input::isKeyPressed(Keyboard::W))
    {
        CameraController::camera->y -= 300 * time.GetSeconds();
    }

    if (Input::isKeyPressed(Keyboard::A))
    {
        CameraController::camera->x -= 300 * time.GetSeconds();
    }

    if (Input::isKeyPressed(Keyboard::S))
    {
        CameraController::camera->y += 300 * time.GetSeconds();
    }

    if (Input::isKeyPressed(Keyboard::D))
    {
        CameraController::camera->x += 300 * time.GetSeconds();
    }

    if (Input::isKeyPressed(Keyboard::E))
    {
        CameraController::camera->zoom_x += 0.1;
        CameraController::camera->zoom_y += 0.1;
    }

    if (Input::isKeyPressed(Keyboard::R))
    {
        CameraController::camera->zoom_x -= 0.1;
        CameraController::camera->zoom_y -= 0.1;
    }
}