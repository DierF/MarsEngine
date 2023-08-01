#include "Runtime/Platform/FileService/FileService.h"

#include <filesystem>
#include <vector>

namespace MarsEngine
{
    std::vector<std::filesystem::path> FileSystem::getFiles(std::filesystem::path const& directory)
    {
        std::vector<std::filesystem::path> files;
        for (auto const& directory_entry : std::filesystem::recursive_directory_iterator {directory})
        {
            if (directory_entry.is_regular_file())
            {
                files.push_back(directory_entry);
            }
        }
        return files;
    }
} // namespace MarsEngine