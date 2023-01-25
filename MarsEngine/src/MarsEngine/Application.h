#pragma once

#include "Core.h"

namespace MarsEngine {

	class ME_API Application
	{
	public:
		Application();

		virtual ~Application();

		void run();
	};

	//to be defined in Client
	Application* createApplication();

}
