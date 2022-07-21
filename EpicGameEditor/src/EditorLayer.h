#pragma once
#include <EpicGameEngine.h>

namespace EpicGameEngine
{
	class EditorLayer : public Layer
	{
	public:
		~EditorLayer();

		void OnAttach() override;
		void OnUpdate(Timestep time) override;
		void OnRender() override;
		void OnImGuiRender() override;

        std::shared_ptr<EpicGameEngine::Scene> activeScene;
        static inline GameObject camera;
	};
}
