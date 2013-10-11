#include "EntityComponents/Entity.h"

namespace ec
{
	Entity::Entity(shared_ptr<EntityManager> manager, EntityType type)
	{
		manager_=weak_ptr<EntityManager>(manager);
		type_=type;
		playerId_=-1;
		predicting_=false;
	}

	shared_ptr<EntityManager> Entity::getManager()
	{
		return manager_.lock();
	}
	
	EntityType Entity::getType()
	{
		return type_;
	}

	int Entity::getId()
	{
		return id_;
	}

	void Entity::setId(int id)
	{
		id_=id;
	}
	
	int Entity::getPlayerId()
	{
		return playerId_;
	}
	
	void Entity::setPlayerId(int id)
	{
		playerId_=id;
	}
	
	bool Entity::predicting()
	{
		return predicting_;
	}
	
	void Entity::setPredicting(bool predicting)
	{
		predicting_=predicting;
	}

	void Entity::update(float deltaTime, int playerId)
	{
		for(auto component : components_)
		{
			if(component)
			{
				component->update(deltaTime, playerId);
			}
		}
	}

	void Entity::onCollision(shared_ptr<Entity> collider, int playerId)
	{
		for(auto component : components_)
		{
			if(component)
			{
				component->onCollision(collider, playerId);
			}
		}
	}
	
	void Entity::pushBack(shared_ptr<Component> component)
	{
		component->setId(components_.size());
		components_.push_back(component);
	}
	
	void Entity::insertAtId(shared_ptr<Component> component)
	{
		int id=component->getId();
		if(id>=size())
		{
			components_.resize(id+1);
		}
		components_[id]=component;
	}
	
	int Entity::size()
	{
		return components_.size();
	}
	
	shared_ptr<Component> Entity::get(int id)
	{
		return components_[id];
	}
	
	void Entity::remove(int id)
	{
		components_[id].reset();
	}
	
	sf::Packet &Entity::appendEntity(sf::Packet& packet, int id)
	{
		//Derived Entities will package their additional attributes and the transformable
		//cout << "Beginning size: " << packet.getDataSize() << endl;
		packet << (int)ContentType::MEntity;
		packet << id_;
		packet << type_;
		packet << playerId_;
		//cout << "After header: " << packet.getDataSize() << endl;
		
		for(auto component : components_)
		{
			component->appendComponent(packet, id);
		}
		//cout << "After components: " << packet.getDataSize() << endl;
		
		append(packet, id);
		packet << (int)ContentType::MEntityEnd;
		//cout << "Derived entity specifics: " << packet.getDataSize() << endl;
		return packet;
	}
}