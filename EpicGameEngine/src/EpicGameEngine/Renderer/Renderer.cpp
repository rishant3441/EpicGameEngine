#include <EpicGameEngine/Renderer/Renderer.h>
#include <EpicGameEngine/Window/Window.h>
#include <EpicGameEngine/Window/WindowsWindow.h>
#include <cassert>
#include <EpicGameEngine/Renderer/Texture.h>
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

            Renderer::texture = GPU_CreateImage(1024, 576, GPU_FORMAT_RGB);
            GPU_LoadTarget(Renderer::texture);
            Renderer::target = Renderer::texture->target;
            GPU_MakeCurrent(Renderer::window, SDL_GetWindowID(WindowsWindow::window));
        }
        unitSize = 50; /// Default size for scaling shapes
    }

    void Renderer::Shutdown()
    {
        if (Renderer::enableDrawingToTexture)
            GPU_FreeTarget(Renderer::target);
        GPU_Quit();
    }

    auto Renderer::GetTarget() -> GPU_Target*
    {
        return Renderer::target;
    }

    void Renderer::Render()
    {
        if (!Renderer::enableDrawingToTexture)
        {
            GPU_Flip(Renderer::target);
        }
        else
        {
            GPU_Flip(Renderer::window);
            GPU_Flip(Renderer::GetTarget());
        }
    }
    
    void Renderer::DrawRect(double x, double y, double w, double h, double rot, SDL_Color color)
    {
        GPU_Rectangle2(Renderer::GetTarget(), { static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h) }, color);
    }

    void Renderer::DrawTexturedRect(double x, double y, double w, double h, const Texture& texture, double rot, SDL_Color color)
    {
        GPU_Rect rect = { static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h) };
        GPU_BlitRectX(texture, nullptr, Renderer::GetTarget(), &rect, rot, 0, 0, GPU_FLIP_VERTICAL);
    }

    void Renderer::DrawFilledRect(double x, double y, double w, double h, double rot, SDL_Color color)
    {
        GPU_RectangleFilled2(Renderer::GetTarget(), { static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h) }, color);
    }
}