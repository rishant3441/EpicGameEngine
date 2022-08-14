#include <EpicGameEngine.h>
#include <EpicGameEngine/EntryPoint.h>
#include "LauncherLayer.h"

class EGELauncherApp : public EpicGameEngine::Application
{
public:
	EGELauncherApp()
	{
		layers.PushLayer(new EpicGameEngine::LauncherLayer());
	}
	~EGELauncherApp()
	{

	}
};

EpicGameEngine::Application* EpicGameEngine::CreateApplication()
{
	return new EGELauncherApp();
}
