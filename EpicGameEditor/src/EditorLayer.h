#pragma once
#include <EpicGameEngine.h>
#include "GameObjectsPanel.h"

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
        glm::vec2 viewportSize{0, 0};
        static inline GameObject camera;
        static inline GameObject rect;

        GameObjectsPanel gameObjectsPanel;
	};
}
