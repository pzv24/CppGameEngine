#include "Entity.h"

Entity::Entity(const size_t& id, EntityTag tag)
	:m_id(id), m_tag(tag){}

bool Entity::isActive() const
{
	return m_active;
}

size_t Entity::getId() const
{
	return m_id;
}

EntityTag Entity::getTag() const
{
	return m_tag;
}

template<typename T>
bool Entity::hasComponent() const
{
	return std::get<T>().exists;
}

template<typename T>
const T& Entity::getComponent() const
{
	return std::get<T>(m_components);
}

template<typename T, typename ...TArgs>
T& Entity::addComponent(TArgs && ...mArgs)
{
	auto& component = std::get<T>();
	component = T(std::forward<TArgs>(mArgs)...);
	component.exists = true;
	return component;
}

template<typename T>
void Entity::removeComponent()
{
	std::get<T>() = T();
}

void Entity::destroyEntity()
{
	m_active = false;
}
