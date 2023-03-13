#include "pch.h"

#include "Renderer.h"
#include "RenderCommand.h"

namespace MarsEngine {

	Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData();

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		m_sceneData->ViewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(std::shared_ptr<Shader> const& shader, std::shared_ptr<VertexArray> const& vertexArray)
	{
		shader->bind();
		shader->uploadUniformMat4("u_viewProjection", m_sceneData->ViewProjectionMatrix);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}