#include "EntityComponents/EntityManager.h"

namespace ec
{
	EntityManager::EntityManager(sf::Vector2u windowSize):
	windowSize_(windowSize)
	{
		
	}

	void EntityManager::update(float deltaTime, int playerId)
	{
		updateEntities(deltaTime, playerId);
		removeEntities();
	}

	void EntityManager::updateEntities(float deltaTime, int playerId)
	{
		for(unsigned int i=0; i<entities_.size(); i++)
		{
			if(entities_[i])
			{
				entities_[i]->update(deltaTime, playerId);
				
				for(unsigned int j=0; j<entities_.size(); j++)
				{
					if(entities_[j] && i!=j)
					{
						sf::FloatRect firstBounds=entities_[i]->getBounds();
						sf::FloatRect secondBounds=entities_[j]->getBounds();
						if(firstBounds.intersects(secondBounds))
						{
							entities_[i]->onCollision(entities_[j], playerId);
						}
					}
				}
			}
		}
	}

	void EntityManager::removeEntities()
	{
		for(auto id : marked_)
		{
			entities_[id].reset();
		}
		marked_.clear();
	}

	void EntityManager::draw(sf::RenderWindow &window)
	{
		for(auto entity : entities_)
		{
			if(entity)
			{
				entity->draw(window);
			}
		}
	}

	void EntityManager::pushBack(shared_ptr<Entity> entity)
	{
		entity->setId(entities_.size());
		entities_.push_back(entity); //memleak?
	}
	
	void EntityManager::insertAtId(shared_ptr<Entity> entity)
	{
		int id=entity->getId();
		if(id>=size())
		{
			entities_.resize(id+1);
		}
		entities_[id]=entity;
	}
	
	int EntityManager::size()
	{
		return entities_.size();
	}

	shared_ptr<Entity> EntityManager::get(int id)
	{
		return entities_[id];
	}
	
	vector<shared_ptr<Entity>> EntityManager::get(EntityType type)
	{
		vector<shared_ptr<Entity>> res;
		for(auto entity : entities_)
		{
			if(entity && entity->getType()==type)
			{
				res.push_back(entity);
			}
		}
		return res;
	}
	
	vector<int> EntityManager::getPlayer(int id)
	{
		vector<int> res;
		for(auto entity : entities_)
		{
			if(entity && entity->getPlayerId()==id)
			{
				res.push_back(entity->getId());
			}
		}
		return res;
	}

	void EntityManager::remove(int id)
	{
		marked_.push_back(id);
	}

	sf::Vector2u EntityManager::getWindowSize()
	{
		return windowSize_;
	}
	
	sf::Packet& EntityManager::operator>>(sf::Packet& packet)
	{
		packet << (int)ContentType::MEntityManager;
		packet << (int)ContentType::MVector2u;
		packet << (sf::Uint32)windowSize_.x;
		packet << (sf::Uint32)windowSize_.y;
		for(auto entity : entities_)
		{
			if(entity)
			{
				entity->appendEntity(packet);
			}
		}
		return packet;
	}
}