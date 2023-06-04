#pragma once

#include "MarsEngine/Core/Core.h"

namespace MarsEngine
{
	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void bind() = 0;

		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t getColorAttachmentRendererID() const = 0;

		virtual FramebufferSpecification const& getSpecification() const = 0;

		static Ref<Framebuffer> create(FramebufferSpecification const& spec);
	};
}