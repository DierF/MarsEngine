#pragma once

#include "MarsEngine/Core/Core.h"
#include <string>

namespace MarsEngine
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t getWidth() const = 0;

		virtual uint32_t getHeight() const = 0;

		virtual uint32_t getRendererID() const = 0;
		
		virtual void setData(void* data, uint32_t size) = 0;

		virtual void bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(Texture const& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> create(uint32_t width, uint32_t height);

		static Ref<Texture2D> create(std::string const& path);
	};
}