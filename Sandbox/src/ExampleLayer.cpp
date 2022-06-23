#include "ExampleLayer.h"
#include <EpicGameEngine.h>

void ExampleLayer::OnAttach()
{
    SDL_Color color;
    color.a = 255;
    color.r = 255;
    color.g = 0;
    color.b = 0;
    EpicGameEngine::Rectangle* rect = new EpicGameEngine::Rectangle(50, 50, 50, 50, "Rectangle", color);
    gameObjects.push_back(rect);
}

void ExampleLayer::OnUpdate()
{
    gameObjects[0]->position.x = EpicGameEngine::Input::GetMouseX();
    gameObjects[0]->position.y = EpicGameEngine::Input::GetMouseY();
}