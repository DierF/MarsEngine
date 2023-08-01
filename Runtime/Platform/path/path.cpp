#include "Runtime/Platform/Path/Path.h"

#include <filesystem>
#include <string>
#include <tuple>
#include <vector>

namespace MarsEngine
{
    std::filesystem::path const Path::getRelativePath(std::filesystem::path const& directory,
                                                 std::filesystem::path const& file_path)
    {
        return file_path.lexically_relative(directory);
    }

    std::vector<std::string> const Path::getPathSegments(std::filesystem::path const& file_path)
    {
        std::vector<std::string> segments;
        for (auto iter = file_path.begin(); iter != file_path.end(); ++iter)
        {
            segments.emplace_back(iter->generic_string());
        }
        return segments;
    }

    std::tuple<std::string, std::string, std::string> const
    Path::getFileExtensions(std::filesystem::path const& file_path)
    {
        return std::make_tuple(file_path.extension().generic_string(),
                          file_path.stem().extension().generic_string(),
                          file_path.stem().stem().extension().generic_string());
    }

    std::string const Path::getFilePureName(std::string const file_full_name)
    {
        std::string file_pure_name = file_full_name;
        auto        pos            = file_full_name.find_first_of('.');
        if (pos != std::string::npos)
        {
            file_pure_name = file_full_name.substr(0, pos);
        }

        return file_pure_name;
    }
} // namespace MarsEngine
