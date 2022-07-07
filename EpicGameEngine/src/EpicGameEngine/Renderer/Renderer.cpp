#include <EpicGameEngine/Renderer/Renderer.h>
#include <EpicGameEngine/Window/Window.h>
#include <EpicGameEngine/Window/WindowsWindow.h>
#include <cassert>
#include <EpicGameEngine/ege_pch.h>
#include <spdlog/spdlog.h>

namespace EpicGameEngine
{
    void Renderer::Init(const WindowData& data)
    {
        GPU_SetDebugLevel(GPU_DEBUG_LEVEL_3);
        if (!enableDrawingToTexture)
        {
            GPU_SetInitWindow(SDL_GetWindowID(WindowsWindow::window));
			Renderer::target = GPU_Init(data.width, data.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        }
        else
        {
		    GPU_SetInitWindow(SDL_GetWindowID(WindowsWindow::window));
            Renderer::window = GPU_Init(data.width, data.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

            Renderer::texture = GPU_CreateImage(800, 600, GPU_FORMAT_RGB);
            GPU_LoadTarget(Renderer::texture);
            Renderer::target = Renderer::texture->target;
            GPU_MakeCurrent(Renderer::window, SDL_GetWindowID(WindowsWindow::window));
        }
    }

    void Renderer::Shutdown()
    {
        if (enableDrawingToTexture)
            GPU_FreeTarget(Renderer::target);
        GPU_Quit();
    }

    auto Renderer::GetTarget() -> GPU_Target*
    {
        return Renderer::target;
    }

    void Renderer::Render()
    {
        if (Renderer::enableDrawingToTexture == false)
        {
            GPU_Flip(Renderer::target);
        }
        else
        {
            GPU_Flip(Renderer::window);
            GPU_Flip(Renderer::GetTarget());
            //texture = GPU_CopyImageFromTarget(Renderer::target);
        }
    }
}