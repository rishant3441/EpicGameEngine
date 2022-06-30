#include <EpicGameEngine/Layers/Layer.h>
#include <EpicGameEngine/ege_pch.h>
#include <spdlog/spdlog.h>

namespace EpicGameEngine
{
    void Layer::OnRender()
    {
        for (int i = 0; i < Layer::gameObjects.size(); i++)
        {
            gameObjects[i]->Render();
        }
    }

    void Layer::OnImGuiRender()
    {

    }
}