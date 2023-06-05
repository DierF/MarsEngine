#pragma once

namespace MarsEngine
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void init() = 0;

		virtual void swapBuffers() = 0;
	};
}