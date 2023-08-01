#pragma once

#include <filesystem>
#include <vector>

namespace MarsEngine
{
    class FileSystem
    {
    public:
        std::vector<std::filesystem::path> getFiles(std::filesystem::path const& directory);
    };
} // namespace MarsEngine