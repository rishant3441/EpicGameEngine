#pragma once
#include <EpicGameEngine.h>

class ExampleLayer : public EpicGameEngine::Layer
{
public:
    void OnAttach() override;
    void OnUpdate(EpicGameEngine::Timestep time) override;
    void OnRender() override;
    static inline std::shared_ptr<EpicGameEngine::Scene> activeScene;
    static inline EpicGameEngine::GameObject camera;
};
