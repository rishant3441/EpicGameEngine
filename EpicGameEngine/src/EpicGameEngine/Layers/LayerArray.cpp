#include <EpicGameEngine/Layers/LayerArray.h>
#include <EpicGameEngine/ege_pch.h>
#include <spdlog/spdlog.h>

namespace EpicGameEngine
{
    void LayerArray::PushLayer(Layer* layer)
    {
        layers.emplace(layers.begin() + layerInsertIndex, layer);
        layerInsertIndex++;
        layers[layerInsertIndex - 1]->OnAttach();
    }
    void LayerArray::PopLayer(Layer* layer)
    {
        auto it = std::find(layers.begin(), layers.begin() + layerInsertIndex, layer);

        if (it != layers.begin() + layerInsertIndex)
        {
            layer->OnDetach();
            layers.erase(it);
            layerInsertIndex--;
        }
    }
}