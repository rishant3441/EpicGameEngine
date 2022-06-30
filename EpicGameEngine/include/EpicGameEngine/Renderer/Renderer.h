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

        inline static GPU_Target* target;
    };
}