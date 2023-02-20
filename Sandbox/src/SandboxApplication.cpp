#include <MarsEngine.h>

class ExampleLayer : public MarsEngine::Layer {

public:
	ExampleLayer() : Layer("Example") {}

	void onUpdate() override {

		//if (MarsEngine::Input::isKeyPressed(ME_KEY_TAB)) {
		//	ME_TRACE("TAB is pressed!");
		//}
	}

	void onEvent(MarsEngine::Event& event) override {
		//ME_TRACE("{0}", event);

		if (event.getEventType() == MarsEngine::EventType::KeyPressed) {
			auto ev = (MarsEngine::KeyPressedEvent&)event;
			ME_TRACE("{0}", (char)ev.getKeyCode());
		}
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