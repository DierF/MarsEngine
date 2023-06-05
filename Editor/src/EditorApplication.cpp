#include "MarsEngine.h"
#include "MarsEngine/Core/EntryPoint.h"
#include "EditorLayer.h"

namespace MarsEngine
{
	class Editor : public Application
	{
	public:
		Editor()
			: Application("Mars Editor")
		{
			pushLayer(new EditorLayer());
		}

		~Editor() = default;
	};

	Application* createApplication()
	{
		return new Editor();
	}
}