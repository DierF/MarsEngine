#include "pch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "MarsEngine/Application.h"

#include "GLFW/glfw3.h"

namespace MarsEngine {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer") {}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::onAttach() {
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDetch() {}
	
	void ImGuiLayer::onUpdate() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::onEvent(Event& event) {}
}