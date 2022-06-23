#include <EpicGameEngine/Renderer/Renderer.h>
#include <EpicGameEngine/Window/Window.h>
#include <EpicGameEngine/ege_pch.h>

namespace EpicGameEngine
{
    void Renderer::Init(const WindowData& data)
    {
        Renderer::target = GPU_Init(data.width, data.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    }

    void Renderer::Shutdown()
    {
        GPU_Quit();
    }

    GPU_Target* Renderer::GetTarget()
    {
        return Renderer::target;
    }
}