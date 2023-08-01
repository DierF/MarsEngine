#include "Runtime/Core/Math/Degree.h"
#include "Runtime/Core/Math/Radian.h"
#include "Runtime/Core/Math/Math.h"

namespace MarsEngine
{
	Degree::Degree(Radian const& r)
		: m_degree(Math::radiansToDegrees(float(r)))
	{
	}

	Degree& Degree::operator=(Radian const& r)
	{
        m_degree = Math::radiansToDegrees(float(r));
		return *this;
	}

	Degree::operator Radian() const
	{
		return Radian(Math::degreesToRadians(m_degree));
	}

	float Degree::toAngleUnits() const
	{
		return Math::degreesToAngleUnits(m_degree);
	}
} // namespace MarsEngine