#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Window/Window.h>

namespace EpicGameEngine
{
    class Renderer
    {
    public:
        static void Init(const WindowData& data);
        static void Shutdown();
        static GPU_Target* GetTarget();

        inline static GPU_Target* target;
    };
}