#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Core.h>
#include <EpicGameEngine/Window/Window.h>

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

        inline static GPU_Target* target;
        inline static GPU_Target* window = nullptr;
        inline static GPU_Image* texture;
        inline static bool enableDrawingToTexture = false;
    };
}