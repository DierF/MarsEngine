#pragma once

#include "MarsEngine/Math/Vector3.h"
#include "Object.h"

namespace MarsEngine
{
    //REFLECTION_TYPE(LevelRes)
        //CLASS(LevelRes, Fields)
    class LevelRes
    {
        //REFLECTION_BODY(LevelRes);

    public:
        Math::Vec3  m_gravity{ 0.f, 0.f, -9.8f };
        std::string m_character_name;

        std::vector<ObjectInstanceRes> m_objects;
    };
} // namespace MarsEngine