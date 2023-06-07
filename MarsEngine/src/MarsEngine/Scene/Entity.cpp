#include "pch.h"

#include "Entity.h"

namespace MarsEngine
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_entityHandle(handle), m_scene(scene)
	{
	}
}