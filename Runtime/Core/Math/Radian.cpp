#include "Runtime/Core/Math/Radian.h"
#include "Runtime/Core/Math/Degree.h"
#include "Runtime/Core/Math/Math.h"

namespace MarsEngine
{
	Radian::Radian(Degree const& d)
		: m_radian(Math::degreesToRadians(float(d)))
	{
	}

	Radian& Radian::operator=(Degree const& d)
	{
        m_radian = Math::degreesToRadians(float(d));
		return *this;
	}

	Radian::operator Degree() const
	{
		return Degree(Math::radiansToDegrees(m_radian));
	}

	float Radian::toAngleUnits() const
	{
		return Math::radiansToAngleUnits(m_radian);
	}
} // namespace MarsEngine