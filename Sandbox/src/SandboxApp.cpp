#include <EpicGameEngine.h>
#include <EpicGameEngine/EntryPoint.h>
#include "ExampleLayer.h"

class Sandbox : public EpicGameEngine::Application
{
public:
	Sandbox()
	{
		layers.PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

auto EpicGameEngine::CreateApplication() -> EpicGameEngine::Application*
{
	return new Sandbox();
}
