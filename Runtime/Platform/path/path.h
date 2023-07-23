#pragma once

#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

namespace MarsEngine
{
    class Path
    {
    public:
        static std::filesystem::path const getRelativePath(std::filesystem::path const& directory,
                                                           std::filesystem::path const& file_path);

        static std::vector<std::string> const getPathSegments(std::filesystem::path const& file_path);

        static std::tuple<std::string, std::string, std::string> const
        getFileExtensions(std::filesystem::path const& file_path);

        static std::string const getFilePureName(std::string const);
    };
} // namespace MarsEngine