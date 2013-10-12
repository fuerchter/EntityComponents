#ifndef COMPONENT
#define COMPONENT

#include "ComponentType.h"

#include "Entity.h"

namespace ec
{
	class Entity;

	class Component
	{
	public:
		Component(shared_ptr<Entity> entity, ComponentType type);
		ComponentType getType();
		int getId();
		void setId(int id);
		
		virtual void update(float deltaTime, int playerId)
		{
		
		}
		virtual void onCollision(shared_ptr<Entity> collider, int playerId=-1)
		{
		
		}
		
		virtual sf::Packet &appendComponent(sf::Packet &packet, int id=-1)
		{
			packet << (int)MessageType::MComponent;
			packet << id_;
			packet << type_;
			
			append(packet, id);
			return packet;
		}
		virtual sf::Packet &append(sf::Packet &packet, int id=-1)
		{
			return packet;
		}
		virtual sf::Packet &extract(sf::Packet &packet, int id=-1)
		{
			return packet;
		}
	protected:
		weak_ptr<Entity> entity_;
		ComponentType type_;
		int id_;
	private:
	};
}

#endif