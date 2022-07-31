#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Layers/Layer.h>
#include <EpicGameEngine/Events/Event.h>
#include <EpicGameEngine/Events/InputEvent.h>

namespace EpicGameEngine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();
        void BeginFrame();
        void OnImGuiRender();
        void EndFrame();

        void SetDarkThemeColors();
    };
}