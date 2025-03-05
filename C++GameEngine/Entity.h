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
	CInput
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

	Entity(const size_t& id, EntityTag tag);

public:
	//public getters
	bool isActive() const;
	size_t getId() const;
	EntityTag getTag() const;

	// component checking
	template<typename T>
	bool hasComponent() const;

	template<typename T>
	const T& getComponent() const;

	//component management
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs);

	template<typename T>
	void removeComponent();

	//entity management
	void destroyEntity();
};