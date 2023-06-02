#pragma once

#include "glm/glm.hpp"
#include "Texture.h"

namespace MarsEngine
{
	class SubTexture2D
	{
	public:
		SubTexture2D(Ref<Texture2D> const& texture, glm::vec2 const& min, glm::vec2 const& max);

		Ref<Texture2D> const getTexture() const { return m_texture; }

		glm::vec2 const* getTextureCoords() const { return m_textureCoords; }

		static Ref<SubTexture2D> createFromCoords(Ref<Texture2D> const& texture, glm::vec2 const& coords, glm::vec2 const& cellSize, glm::vec2 const& spriteSize = { 1, 1 });

	private:
		Ref<Texture2D> m_texture;

		glm::vec2 m_textureCoords[4];
	};
}