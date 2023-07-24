#pragma once

#include "Runtime/Function/Render/DebugDraw/DebugDrawGroup.h"

#include <vector>
#include <string>
#include <mutex>

namespace MarsEngine
{
    class DebugDrawContext
    {
    public:
        std::vector<DebugDrawGroup*> m_debug_draw_groups;
        DebugDrawGroup* tryGetOrCreateDebugDrawGroup(std::string const& name);
        void clear();
        void tick(float delta_time);
    
    private:
        std::mutex m_mutex;
        void removeDeadPrimitives(float delta_time);
    };
} // namespace MarsEngine