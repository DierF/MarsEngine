#include "pch.h"

#include "SubTexture2D.h"

namespace MarsEngine
{
	SubTexture2D::SubTexture2D(Ref<Texture2D> const& texture, glm::vec2 const& min, glm::vec2 const& max)
		: m_texture(texture)
	{
		m_textureCoords[0] = { min.x, min.y };
		m_textureCoords[1] = { max.x, min.y };
		m_textureCoords[2] = { max.x, max.y };
		m_textureCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::createFromCoords(Ref<Texture2D> const& texture, glm::vec2 const& coords, glm::vec2 const& cellSize, glm::vec2 const& spriteSize)
	{
		glm::vec2 min = { (coords.x * cellSize.x) / texture->getWidth(), (coords.y * cellSize.y) / texture->getHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->getWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->getHeight() };
		return createRef<SubTexture2D>(texture, min, max);
	}
}