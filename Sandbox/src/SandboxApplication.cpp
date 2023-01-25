#include <MarsEngine.h>

class Sandbox : public MarsEngine::Application {
public:
	Sandbox() {}

	~Sandbox() {}

	
};

MarsEngine::Application* MarsEngine::createApplication() {
	return new Sandbox();
}