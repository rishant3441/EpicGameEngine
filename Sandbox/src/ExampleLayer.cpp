#include "ExampleLayer.h"
#include <EpicGameEngine.h>

void ExampleLayer::OnAttach()
{
    SDL_Color color;
    color.a = 255;
    color.r = 255;
    color.g = 0;
    color.b = 0;
    auto* rect = new EpicGameEngine::Rectangle(600, 350, 50, 50, "Rectangle", color);
    gameObjects.push_back(rect);
    EpicGameEngine::CameraController::CreateCamera();
}

using namespace EpicGameEngine;

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