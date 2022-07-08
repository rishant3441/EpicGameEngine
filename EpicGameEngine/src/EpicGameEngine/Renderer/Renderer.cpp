#include <EpicGameEngine/Renderer/Renderer.h>
#include <EpicGameEngine/Window/Window.h>
#include <EpicGameEngine/Renderer/Texture.h>
#include <EpicGameEngine/ege_pch.h>
#include <spdlog/spdlog.h>

namespace EpicGameEngine
{
    void Renderer::Init(const WindowData& data)
    {
        unitSize = 50; /// Default size for scaling shapes
        Renderer::target = GPU_Init(data.width, data.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    }

    void Renderer::Shutdown()
    {
        GPU_Quit();
    }

    auto Renderer::GetTarget() -> GPU_Target*
    {
        return Renderer::target;
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