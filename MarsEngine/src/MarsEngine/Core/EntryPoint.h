#pragma once

#ifdef ME_PLATFORM_WINDOWS

extern MarsEngine::Application* MarsEngine::createApplication();

int main(int argc, char** argv) {

	MarsEngine::Log::init();

	ME_PROFILE_BEGIN_SESSION("Startup", "MarsEngineProfile-Startup.json");
	auto app = MarsEngine::createApplication();
	ME_PROFILE_END_SESSION();

	ME_PROFILE_BEGIN_SESSION("Runtime", "MarsEngineProfile-Runtime.json");
	app->run();
	ME_PROFILE_END_SESSION();

	ME_PROFILE_BEGIN_SESSION("Shutdown", "MarsEngineProfile-Shutdown.json");
	delete app;
	ME_PROFILE_END_SESSION();
}

#endif