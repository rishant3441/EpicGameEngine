#include <EpicGameEngine.h>
#include <stdio.h>

class Sandbox : public EpicGameEngine::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}

	void Run()
	{
		EpicGameEngine::WindowsWindow window = EpicGameEngine::WindowsWindow::WindowsWindow();
	
		while (true)
		{

		}
	}
};

EpicGameEngine::Application* EpicGameEngine::CreateApplication()
{
	return new Sandbox();
}
