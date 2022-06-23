#pragma once
#include <EpicGameEngine/ege_pch.h>
#include <EpicGameEngine/Layers/Layer.h>
#include <vector>

namespace EpicGameEngine
{
    class EPICGAMEENGINE_API LayerArray
    {
    public:
        LayerArray() = default;
        ~LayerArray() = default;

        void PushLayer(Layer* layer);
        void PopLayer(Layer* layer);

        std::vector<Layer*> layers;
    private:
        unsigned int layerInsertIndex = 0;
    };
}