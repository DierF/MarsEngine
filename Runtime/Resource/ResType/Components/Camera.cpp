#include "Runtime/Resource/ResType/Components/Camera.h"
#include "Runtime/Core/Base/Macro.h"

namespace MarsEngine
{
    CameraComponentRes::CameraComponentRes(CameraComponentRes const& res)
    {
        std::string const& camera_type_name = res.m_parameter.getTypeName();
        if (camera_type_name == "FirstPersonCameraParameter")
        {
            m_parameter = MARS_REFLECTION_NEW(FirstPersonCameraParameter);
            MARS_REFLECTION_DEEP_COPY(FirstPersonCameraParameter, m_parameter, res.m_parameter);
        }
        else if (camera_type_name == "ThirdPersonCameraParameter")
        {
            m_parameter = MARS_REFLECTION_NEW(ThirdPersonCameraParameter);
            MARS_REFLECTION_DEEP_COPY(ThirdPersonCameraParameter, m_parameter, res.m_parameter);
        }
        else if (camera_type_name == "FreeCameraParameter")
        {
            m_parameter = MARS_REFLECTION_NEW(FreeCameraParameter);
            MARS_REFLECTION_DEEP_COPY(FreeCameraParameter, m_parameter, res.m_parameter);
        }
        else
        {
            LOG_ERROR("invalid camera type");
        }
    }

    CameraComponentRes::~CameraComponentRes() { MARS_REFLECTION_DELETE(m_parameter); }
} // namespace MarsEngine