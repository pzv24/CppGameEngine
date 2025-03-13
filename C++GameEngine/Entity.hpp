#pragma once
#include "Components.hpp"
#include <string>
#include <tuple>

class EntityManager;

using ComponentTuple = std::tuple<
	CTransform,
	CRigidbody,
	CCircle,
	CCircleCollider,
	CBoxCollider,
	CInput,
	CRectangle
>;

enum EntityTag
{
	defaultEntity,
	player,
	enemy,
	projectile,
	collidableTerrain,
	decorativeTerrain,
};


class Entity
{
	friend class EntityManager;

	ComponentTuple m_components;
	bool m_active = true;
	EntityTag m_tag = defaultEntity;
	size_t m_id = 0;

	Entity(const size_t& id, EntityTag tag)
		:m_id(id), m_tag(tag){}

public:
	
	bool isActive() const
	{
		return m_active;
	}

	size_t getId() const
	{
		return m_id;
	}

	EntityTag getTag() const
	{
		return m_tag;
	}
	const char* getTagName()
	{
		switch (m_tag)
		{
		case defaultEntity: return "defaultEntity";
		case player: return "player";
		case enemy: return "enemy";
		case projectile: return "projectile";
		case collidableTerrain: return "collidableTerrain";
		case decorativeTerrain: return "decorativeTerrain";
		}
	}

	template<typename T>
	bool hasComponent() const
	{
		return getComponent<T>().exists;
	}

	template<typename T>
	const T& getComponent() const
	{
		return std::get<T>(m_components);
	}

	template<typename T>
	T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.exists = true;
		return component;
	}

	template<typename T>
	void removeComponent()
	{
		getComponent<T>() = T();
	}

	void destroyEntity()
	{
		m_active = false;
	}
};
