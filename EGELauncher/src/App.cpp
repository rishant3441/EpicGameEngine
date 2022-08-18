#include <EpicGameEngine.h>
#include <EpicGameEngine/EntryPoint.h>
#include "LauncherLayer.h"

class EGELauncherApp : public EpicGameEngine::Application
{
public:
	EGELauncherApp(int argc, char** argv)
	{
		layers.PushLayer(new EpicGameEngine::LauncherLayer(argc, argv));
	}
	~EGELauncherApp()
	{

	}
};

EpicGameEngine::Application* EpicGameEngine::CreateApplication(int argc, char** argv)
{
	return new EGELauncherApp(argc, argv);
}
