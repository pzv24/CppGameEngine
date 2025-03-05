#pragma once
#include "Entity.h"
#include <map>

using EntityVector = std::vector<std::shared_ptr<Entity>>;

class EntityManager
{
private:
	EntityVector m_entities;
	EntityVector m_entityAddBuffer;
	std::map<EntityTag, EntityVector> m_entityTagMap;
	size_t m_entitiesCreated = 0;

	EntityManager() = default;
public:
	void update();
	std::shared_ptr<Entity> addEntity(EntityTag tag);
	const EntityVector& getEntities();
	const EntityVector& getEntities(EntityTag tag);
};

