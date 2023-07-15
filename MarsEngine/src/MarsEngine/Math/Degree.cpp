#include "Degree.h"
#include "Math.h"

namespace MarsEngine
{
	namespace Math
	{
		Degree::Degree(Radian const& r)
			: m_degree(radiansToDegrees(float(r)))
		{
		}

		Degree& Degree::operator=(Radian const& r)
		{
			m_degree = radiansToDegrees(float(r));
			return *this;
		}

		Degree::operator Radian() const
		{
			return Radian(degreesToRadians(m_degree));
		}

		float Degree::toAngleUnits() const
		{
			return degreesToAngleUnits(m_degree);
		}
	} // namespace Math
} // namespace MarsEngine