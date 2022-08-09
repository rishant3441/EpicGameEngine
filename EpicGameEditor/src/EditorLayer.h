#pragma once
#include <EpicGameEngine.h>
#include "GameObjectsPanel.h"
#include <dear_sink.h>

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
        virtual void OnEvent(std::shared_ptr<Event> e) override;

        bool OnScrollEvent(MouseScrolledEvent& event);

        void Toolbar();

        std::shared_ptr<EpicGameEngine::Scene> activeScene;
        glm::vec2 viewportSize{0, 0};

        EditorCamera editorCamera;

        static inline GameObject camera;
        static inline GameObject rect;

        GameObjectsPanel gameObjectsPanel;

        inline static dear_sink_mt_t ssink;
	};
}
