#pragma once

#include "Runtime/Core/Meta/Reflection/Reflection.h"
#include "Runtime/Core/Math/Vector3.h"
#include "Runtime/Resource/ResType/Common/Object.h"

namespace MarsEngine
{
    REFLECTION_TYPE(LevelRes)
    CLASS(LevelRes, Fields)
    {
        REFLECTION_BODY(LevelRes);

    public:
        Vector3     m_gravity {0.f, 0.f, -9.8f};
        std::string m_character_name;

        std::vector<ObjectInstanceRes> m_objects;
    };
} // namespace MarsEngine