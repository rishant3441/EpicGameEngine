#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Window/Window.h>

namespace EpicGameEngine
{
    class Renderer
    {
    public:
        static void Init(const WindowData& data);
        static void Shutdown();
        static auto GetTarget() -> GPU_Target*;

        inline static GPU_Target* target;
    };
}