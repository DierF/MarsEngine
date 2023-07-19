#pragma once

#ifdef ME_PLATFORM_WINDOWS

extern MarsEngine::Application* MarsEngine::createApplication();

int main(int argc, char** argv)
{
	MarsEngine::Log::init();

	auto app = MarsEngine::createApplication();

	app->run();

	delete app;

	return 0;
}

#endif