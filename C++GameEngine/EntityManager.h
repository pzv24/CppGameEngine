#pragma once
#include "Entity.hpp"
#include <map>
#include "CollisionData.hpp"

using EntityVector = std::vector<std::shared_ptr<Entity>>;

class EntityManager
{
private:
	EntityVector m_entities;
	EntityVector m_entityAddBuffer;
	std::map<EntityTag, EntityVector> m_entityTagMap;
	size_t m_entitiesCreated = 0;
	void RemoveInactiveFromVector(EntityVector& entities);
public:
	EntityManager() = default;
	void update();
	std::shared_ptr<Entity> addEntity(EntityTag tag);
	const EntityVector& getEntities();
	const EntityVector& getEntities(EntityTag tag);
	bool hasPlayer();
};

