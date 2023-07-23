#include "Runtime/Resource/AssetManager/AssetManager.h"
#include "Runtime/Resource/ConfigManager/ConfigManager.h"
#include "Runtime/Function/Global/GlobalContext.h"

#include <filesystem>

namespace MarsEngine
{
    std::filesystem::path AssetManager::getFullPath(std::string const& relative_path) const
    {
        return std::filesystem::absolute(g_runtime_global_context.m_config_manager->getRootFolder() / relative_path);
    }
} // namespace MarsEngine