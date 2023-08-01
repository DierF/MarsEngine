#pragma once

#include <filesystem>

namespace MarsEngine
{
    struct EngineInitParams;

    class ConfigManager
    {
    public:
        void initialize(std::filesystem::path const& config_file_path);

        std::filesystem::path const& getRootFolder() const;
        std::filesystem::path const& getAssetFolder() const;
        std::filesystem::path const& getSchemaFolder() const;
        std::filesystem::path const& getEditorBigIconPath() const;
        std::filesystem::path const& getEditorSmallIconPath() const;
        std::filesystem::path const& getEditorFontPath() const;

#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
        std::filesystem::path const& getJoltPhysicsAssetFolder() const;
#endif

        std::string const& getDefaultWorldUrl() const;
        std::string const& getGlobalRenderingResUrl() const;
        std::string const& getGlobalParticleResUrl() const;

    private:
        std::filesystem::path m_root_folder;
        std::filesystem::path m_asset_folder;
        std::filesystem::path m_schema_folder;
        std::filesystem::path m_editor_big_icon_path;
        std::filesystem::path m_editor_small_icon_path;
        std::filesystem::path m_editor_font_path;

#ifdef ENABLE_PHYSICS_DEBUG_RENDERER
        std::filesystem::path m_jolt_physics_asset_folder;
#endif

        std::string m_default_world_url;
        std::string m_global_rendering_res_url;
        std::string m_global_particle_res_url;
    };
} // namespace MarsEngine
