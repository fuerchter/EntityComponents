#ifndef PACKETFACTORY
#define PACKETFACTORY

#include "enet/enet.h"

#include "EntityManager.h"
#include "Sprite.h"
#include "Movement.h"

namespace ec
{
	class EntityManager;
	class Entity;
	class Component;

	class PacketFactory
	{
	public:
		static ENetPacket *sfToENet(sf::Packet packet);
		static sf::Packet eNetToSf(ENetPacket *packet);
		
		//Append and extract for some SFML built-in types
		static void append(sf::Packet &packet, sf::IntRect content, bool header=true);
		static sf::IntRect extractIntRect(sf::Packet &packet);
		
		static void append(sf::Packet &packet, sf::Color content, bool header=true);
		static sf::Color extractColor(sf::Packet &packet);
		
		static void append(sf::Packet &packet, sf::Vector2f content, bool header=true);
		static sf::Vector2f extractVector2f(sf::Packet &packet);
		
		static void append(sf::Packet &packet, sf::Vector2u content, bool header=true);
		static sf::Vector2u extractVector2u(sf::Packet &packet);
		
		static void append(sf::Packet &packet, sf::Sprite sprite, string textureName, bool header=true);
		static sf::Sprite extractSprite(sf::Packet &packet, string textureName, ResourceManager &resources);
		
		//Construct EntityManager from packet
		static shared_ptr<EntityManager> extractEntities(sf::Packet &packet, ResourceManager &resources); //append/extract onto already existing objects?
		static shared_ptr<Entity> extractEntity(sf::Packet &packet, shared_ptr<EntityManager> entities, ResourceManager &resources, int extractionId=-1);
		static shared_ptr<Component> extractComponent(sf::Packet &packet, shared_ptr<Entity> entity, int extractionId=-1);
	private:
	};
}

#endif