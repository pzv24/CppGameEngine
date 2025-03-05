#include "EntityManager.h"

void EntityManager::update()
{
	//adding entities to the actual vectors
	for (std::shared_ptr<Entity> e : m_entityAddBuffer)
	{
		m_entities.push_back(e);
		m_entityTagMap[e->getTag()].push_back(e);
	}

	//removing inactive entities from lists
	//m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> e) { return !e->isActive(); }), m_entities.end())
	for (auto it = m_entities.begin(); it != m_entities.end();)
	{
		if ((*it)->isActive())
		{
			m_entities.erase(it);
			m_entityTagMap.erase((*it)->getTag());
		}
		else
		{
			++it;
		}
	}
	//reset the add buffer
	m_entityAddBuffer.clear();
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

	//add entity to tag map
	m_entityTagMap[tag].push_back(e);

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
