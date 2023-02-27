#pragma once

namespace MarsEngine {

	class ME_API GraphicsContext {

	public:
		virtual void init() = 0;

		virtual void swapBuffers() = 0;
	};
}