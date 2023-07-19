#pragma once

#include "Entity.h"

namespace MarsEngine
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& getComponent()
		{
			return m_entity.getComponent<T>();
		}

	protected:
		virtual void onCreate() {}

		virtual void onDestroy() {}

		virtual void onUpdate(Timestep ts) {}

	private:
		Entity m_entity;

		friend class Scene;
	};
}