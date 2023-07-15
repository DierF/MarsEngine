#include "Radian.h"
#include "Math.h"

namespace MarsEngine
{
namespace Math
{
	Radian::Radian(Degree const& d)
		: m_radian(degreesToRadians(float(d)))
	{
	}

	Radian& Radian::operator=(Degree const& d)
	{
		m_radian = degreesToRadians(float(d));
		return *this;
	}

	Radian::operator Degree() const
	{
		return Degree(radiansToDegrees(m_radian));
	}

	float Radian::toAngleUnits() const
	{
		return radiansToAngleUnits(m_radian);
	}
} // namespace Math
} // namespace MarsEngine