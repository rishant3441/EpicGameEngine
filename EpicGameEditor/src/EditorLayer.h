#pragma once
#include <EpicGameEngine.h>

namespace EpicGameEngine
{
	class EditorLayer : public Layer
	{
	public:
		~EditorLayer();

		void OnAttach();
		void OnUpdate(Timestep time);
		void OnImGuiRender();
	};
}
