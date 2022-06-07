#pragma once

#ifdef EGE_PLATFORM_WINDOWS

extern EpicGameEngine::Application* EpicGameEngine::CreateApplication();
	
int main(int argc, char** argv)
{
	auto app = EpicGameEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif
