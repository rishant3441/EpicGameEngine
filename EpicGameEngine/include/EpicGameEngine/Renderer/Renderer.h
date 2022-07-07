#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Core.h>
#include <EpicGameEngine/Window/Window.h>
#include <EpicGameEngine/Renderer/Texture.h>

namespace EpicGameEngine
{
    class EPICGAMEENGINE_API Renderer
    {
    public:
        static void Init(const WindowData& data);
        static void Shutdown();
        static auto GetTarget() -> GPU_Target*;

        static void DrawRect(double x, double y, double w, double h, double rot = 0, SDL_Color color = { 255, 0, 0, 255 });
        static void DrawTexturedRect(double x, double y, double w, double h, const Texture& texture, double rot = 0, SDL_Color color = { 255, 0, 0, 255 });
        static void DrawFilledRect(double x, double y, double w, double h, double rot = 0, SDL_Color color = { 255, 0, 0, 255 });

        inline static GPU_Target* target;
    };
}