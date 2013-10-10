#ifndef ENTITY
#define ENTITY

#include "SFML/Network.hpp"

#include "ResourceManager/ResourceManager.h"

#include "EntityType.h"
#include "ContentType.h"

#include "EntityManager.h"
#include "Component.h"

namespace ec
{
	class EntityManager;
	class Component;

	class Entity
	{
	public:
		Entity(shared_ptr<EntityManager> manager, EntityType type);
		
		shared_ptr<EntityManager> getManager();
		EntityType getType();
		int getId();
		void setId(int id);
		int getPlayerId();
		void setPlayerId(int id);
		bool predicting();
		void setPredicting(bool predicting);
		
		void update(float deltaTime, int playerId=-1);
		virtual sf::FloatRect getBounds() //TODO: Check whether two (0, 0, 0, 0) FloatRects intersect
		{
			return sf::FloatRect();
		}
		void onCollision(shared_ptr<Entity> collider);
		virtual void draw(sf::RenderWindow &window)
		{
		
		}
		
		void pushBack(shared_ptr<Component> component);
		void insertAtId(shared_ptr<Component> component);
		int size();
		shared_ptr<Component> get(int id);
		void remove(int id); //TODO: Check whether they have to be marked as well
		
		sf::Packet &appendEntity(sf::Packet &packet, int id=-1); //Default Entity appending method
		//Override this if derived entity has attributes that need to be sent
		virtual sf::Packet &append(sf::Packet &packet, int id=-1)
		{
			return packet;
		}
		virtual sf::Packet &extract(sf::Packet &packet, ResourceManager &resources, int id=-1)
		{
			return packet;
		}
		
		shared_ptr<sf::Transformable> transformable;
	protected:
		weak_ptr<EntityManager> manager_; //weak_ptr to break circular reference
		EntityType type_;
		int id_;
		
		//Multiplayer-related
		int playerId_;
		bool predicting_;
		
		vector<shared_ptr<Component>> components_;
	private:
		
	};
}

#endif