#include <EpicGameEngine.h>
#include <EpicGameEngine/EntryPoint.h>
#include "EditorLayer.h"

namespace EpicGameEngine
{
	class EpicGameEditor : public Application
	{
	public:
		EpicGameEditor(int argc, char** argv)
		{
			layers.PushLayer(new EditorLayer(argc, argv));
		}

		~EpicGameEditor()
		{
		}
	};

	Application* CreateApplication(int argc, char** argv)
	{
		return new EpicGameEditor(argc, argv);
	}
}
