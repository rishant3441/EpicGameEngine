#include <EpicGameEngine.h>

class Sandbox : public EpicGameEngine::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

EpicGameEngine::Application* EpicGameEngine::CreateApplication()
{
	return new Sandbox();
}
