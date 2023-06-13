#pragma once

#include <string>

namespace MarsEngine
{
	class FileDialog
	{
	public:
		//Both return empty string if cancelled
		static std::string openFile(char const* filter);

		static std::string saveFile(char const* filter);
	};
}