#include "EntityManager.h"


void EntityManager::update()
{
	//adding entities to the actual vectors
	for (std::shared_ptr<Entity> e : m_entityAddBuffer)
	{
		m_entities.push_back(e);
		m_entityTagMap[e->getTag()].push_back(e);
		std::cout << "Entity count: " << m_entities.size() << "\n There are now " << m_entityTagMap[e->getTag()].size() << " in the " << e->getTagName() << " tag map";
	}

	//removing inactive entities from lists
	RemoveInactiveFromVector(m_entities);
	for (auto& pair : m_entityTagMap)
	{
		RemoveInactiveFromVector(pair.second);
	}
	m_entityAddBuffer.clear();
}

void EntityManager::RemoveInactiveFromVector(EntityVector& entities)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [](std::shared_ptr<Entity> e) { return !e->isActive(); }), entities.end());
}
std::shared_ptr<Entity> EntityManager::addEntity(EntityTag tag)
{
	// create entity pointer
	auto e = std::shared_ptr<Entity>(new Entity(m_entitiesCreated++, tag));

	// add to main entity list
	m_entityAddBuffer.push_back(e);

	//if the vector in the tag map has not been used, initialize an empty entity vector
	if (m_entityTagMap.find(tag) == m_entityTagMap.end())
	{
		m_entityTagMap[tag] = EntityVector();
	}

	return e;
}

const EntityVector& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVector& EntityManager::getEntities(EntityTag tag)
{
	//if the vector in the tag map has not been used, initialize an empty entity vector
	if (m_entityTagMap.find(tag) == m_entityTagMap.end())
	{
		m_entityTagMap[tag] = EntityVector();
	}
	return m_entityTagMap[tag];
}
