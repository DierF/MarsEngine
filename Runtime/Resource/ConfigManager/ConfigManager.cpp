#include "Runtime/Resource/ConfigManager/ConfigManager.h"
#include "Runtime/Engine.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace MarsEngine
{
    void ConfigManager::initialize(std::filesystem::path const& config_file_path)
    {
        // read configs
        std::ifstream config_file(config_file_path);
        std::string   config_line;
        while (std::getline(config_file, config_line))
        {
            size_t seperate_pos = config_line.find_first_of('=');
            if (seperate_pos > 0 && seperate_pos < (config_line.length() - 1))
            {
                std::string name  = config_line.substr(0, seperate_pos);
                std::string value = config_line.substr(seperate_pos + 1, config_line.length() - seperate_pos - 1);
                if (name == "BinaryRootFolder")
                {
                    m_root_folder = config_file_path.parent_path() / value;
                }
                else if (name == "AssetFolder")
                {
                    m_asset_folder = m_root_folder / value;
                }
                else if (name == "SchemaFolder")
                {
                    m_schema_folder = m_root_folder / value;
                }
                else if (name == "DefaultWorld")
                {
                    m_default_world_url = value;
                }
                else if (name == "BigIconFile")
                {
                    m_editor_big_icon_path = m_root_folder / value;
                }
                else if (name == "SmallIconFile")
                {
                    m_editor_small_icon_path = m_root_folder / value;
                }
                else if (name == "FontFile")
                {
                    m_editor_font_path = m_root_folder / value;
                }
                else if (name == "GlobalRenderingRes")
                {
                    m_global_rendering_res_url = value;
                }
                else if (name == "GlobalParticleRes")
                {
                    m_global_particle_res_url = value;
                }
#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
                else if (name == "JoltAssetFolder")
                {
                    m_jolt_physics_asset_folder = m_root_folder / value;
                }
#endif
            }
        }
    }

    std::filesystem::path const& ConfigManager::getRootFolder() const { return m_root_folder; }

    std::filesystem::path const& ConfigManager::getAssetFolder() const { return m_asset_folder; }

    std::filesystem::path const& ConfigManager::getSchemaFolder() const { return m_schema_folder; }

    std::filesystem::path const& ConfigManager::getEditorBigIconPath() const { return m_editor_big_icon_path; }

    std::filesystem::path const& ConfigManager::getEditorSmallIconPath() const { return m_editor_small_icon_path; }

    std::filesystem::path const& ConfigManager::getEditorFontPath() const { return m_editor_font_path; }

    std::string const& ConfigManager::getDefaultWorldUrl() const { return m_default_world_url; }

    std::string const& ConfigManager::getGlobalRenderingResUrl() const { return m_global_rendering_res_url; }

    std::string const& ConfigManager::getGlobalParticleResUrl() const { return m_global_particle_res_url; }

#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
    std::filesystem::path const& ConfigManager::getJoltPhysicsAssetFolder() const { return m_jolt_physics_asset_folder; }
#endif

} // namespace MarsEngine
