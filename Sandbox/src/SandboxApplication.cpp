#include <MarsEngine.h>

class ExampleLayer : public MarsEngine::Layer {

public:
	ExampleLayer() : Layer("Example") {}

	void onUpdate() override {
		ME_INFO("ExampleLayer.update");
	}

	void onEvent(MarsEngine::Event& event) override {
		ME_TRACE("{0}", event);
	}
};

class Sandbox : public MarsEngine::Application {

public:
	Sandbox() {
		pushLayer(new ExampleLayer());
	}

	~Sandbox() {}

	
};

MarsEngine::Application* MarsEngine::createApplication() {
	return new Sandbox();
}