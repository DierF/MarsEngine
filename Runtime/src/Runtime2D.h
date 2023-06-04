#pragma once

#include "MarsEngine.h"

#include "ParticleSystem.h"

class Runtime2D : public MarsEngine::Layer
{
public:
	Runtime2D();

	~Runtime2D() = default;

	virtual void onAttach() override;

	virtual void onDetach() override;

	virtual void onUpdate(MarsEngine::Timestep ts) override;

	virtual void onImGuiRender() override;

	virtual void onEvent(MarsEngine::Event& e) override;

private:
	MarsEngine::OrthographicCameraController m_cameraController;

	MarsEngine::Ref<MarsEngine::VertexArray> m_squareVA;
	MarsEngine::Ref<MarsEngine::Shader> m_flatColorShader;

	MarsEngine::Ref<MarsEngine::Texture2D> m_checkerboardTexture;
	MarsEngine::Ref<MarsEngine::Texture2D> m_spriteSheet;
	MarsEngine::Ref<MarsEngine::SubTexture2D> m_subTexture;

	glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleSystem m_particleSystem;
	ParticleProps m_particle;
};