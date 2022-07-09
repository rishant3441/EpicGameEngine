#include <EpicGameEngine.h>
#include <EpicGameEngine/EntryPoint.h>
#include "EditorLayer.h"

namespace EpicGameEngine
{
	class EpicGameEditor : public Application
	{
	public:	
		EpicGameEditor()
		{
			layers.PushLayer(new EditorLayer());
		}

		~EpicGameEditor()
		{
		}
	};

	EpicGameEngine::Application* EpicGameEngine::CreateApplication()
	{
		return new EpicGameEditor();
	}
}
