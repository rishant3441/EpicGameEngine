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
        
        static void Render(); 

        static void ToggleDrawingToTexture()
        {
            if (enableDrawingToTexture == false)
            {
                enableDrawingToTexture = true;
            }
            else
                enableDrawingToTexture = false;
        }

        static void DrawRect(double x, double y, double w, double h, double rot = 0, SDL_Color color = { 255, 0, 0, 255 });
        static void DrawTexturedRect(double x, double y, double w, double h, const Texture& texture, double rot = 0, SDL_Color color = { 255, 0, 0, 255 });
        static void DrawFilledRect(double x, double y, double w, double h, double rot = 0, SDL_Color color = { 255, 0, 0, 255 });

        inline static GPU_Target* target;
        inline static GPU_Target* window = nullptr;
        inline static GPU_Image* texture;
        inline static bool enableDrawingToTexture = false;
        inline static int unitSize;
    };
}