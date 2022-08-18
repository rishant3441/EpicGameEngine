#pragma once

#ifdef EGE_PLATFORM_WINDOWS

extern EpicGameEngine::Application* EpicGameEngine::CreateApplication(int argc, char** argv);
	
int main(int argc, char** argv)
{
	auto app = EpicGameEngine::CreateApplication(argc, argv);
	app->Run(argc, argv);
	delete app;

	return 0;
}

#endif
