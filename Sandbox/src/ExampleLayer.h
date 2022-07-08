#pragma once
#include <EpicGameEngine.h>

class ExampleLayer : public EpicGameEngine::Layer
{
public:
    void OnAttach() override;
    void OnUpdate(EpicGameEngine::Timestep time) override;
    void OnRender() override;
    std::shared_ptr<EpicGameEngine::Scene> activeScene;

};
