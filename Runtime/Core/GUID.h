#pragma once

#include <xhash>

namespace MarsEngine
{
	class GUID
	{
	public:
		GUID();

		GUID(uint64_t guid);

		GUID(GUID const&) = default;

		operator uint64_t() const { return m_GUID; }

	private:
		uint64_t m_GUID;
	};
}

namespace std
{
	template<>
	struct hash<MarsEngine::GUID>
	{
		size_t operator()(MarsEngine::GUID const& guid) const
		{
			return hash<uint64_t>()((uint64_t)guid);
		}
	};
}