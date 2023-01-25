#pragma once

#ifdef ME_PLATFORM_WINDOWS

extern MarsEngine::Application* MarsEngine::createApplication();

int main(int argc, char** argv) {
	printf("MarsEngine");
	auto app = MarsEngine::createApplication();
	app->run();
	delete app;
}

#endif