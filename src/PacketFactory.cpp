#include "EntityComponents/PacketFactory.h"

namespace ec
{
	ENetPacket *PacketFactory::sfToENet(sf::Packet packet)
	{
		return enet_packet_create(packet.getData(), packet.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
	}

	sf::Packet PacketFactory::eNetToSf(ENetPacket *packet)
	{
		sf::Packet res;
		res.append(packet->data, packet->dataLength);
		return res;
	}

	void PacketFactory::append(sf::Packet &packet, sf::IntRect content)
	{
		packet << (int)ContentType::MIntRect;
		
		packet << content.left;
		packet << content.top;
		packet << content.width;
		packet << content.height;
	}
	
	sf::IntRect PacketFactory::extractIntRect(sf::Packet &packet)
	{
		sf::IntRect res;
		packet >> res.left;
		packet >> res.top;
		packet >> res.width;
		packet >> res.height;
		return res;
	}

	void PacketFactory::append(sf::Packet &packet, sf::Color content)
	{
		packet << (int)ContentType::MColor;
		
		packet << content.r;
		packet << content.g;
		packet << content.b;
		packet << content.a;
	}
	
	sf::Color PacketFactory::extractColor(sf::Packet &packet)
	{
		sf::Color res;
		packet >> res.r;
		packet >> res.g;
		packet >> res.b;
		packet >> res.a;
		return res;
	}

	void PacketFactory::append(sf::Packet &packet, sf::Vector2f content)
	{
		packet << (int)ContentType::MVector2f;
		
		packet << content.x;
		packet << content.y;
	}
	
	sf::Vector2f PacketFactory::extractVector2f(sf::Packet &packet)
	{
		sf::Vector2f res;
		packet >> res.x;
		packet >> res.y;
		return res;
	}
	
	void PacketFactory::append(sf::Packet &packet, sf::Vector2u content)
	{
		packet << (int)ContentType::MVector2u;
		
		packet << content.x;
		packet << content.y;
	}
	
	sf::Vector2u PacketFactory::extractVector2u(sf::Packet &packet)
	{
		sf::Vector2u res;
		packet >> res.x;
		packet >> res.y;
		return res;
	}

	void PacketFactory::append(sf::Packet &packet, sf::Sprite sprite, string textureName)
	{
		packet << (int)ContentType::MSprite;
		
		packet << textureName;
		append(packet, sprite.getTextureRect());
		append(packet, sprite.getColor());
		
		append(packet, sprite.getPosition());
		packet << sprite.getRotation();
		append(packet, sprite.getScale());
		append(packet, sprite.getOrigin());
	}

	sf::Sprite PacketFactory::extractSprite(sf::Packet &packet, string textureName, ResourceManager &resources)
	{
		int type;
		packet >> type;
		sf::IntRect textureRect=extractIntRect(packet);
		
		packet >> type;
		sf::Color color=extractColor(packet);
		
		packet >> type;
		sf::Vector2f position=extractVector2f(packet);
		
		float rotation;
		packet >> rotation;
		
		packet >> type;
		sf::Vector2f scale=extractVector2f(packet);
		
		packet >> type;
		sf::Vector2f origin=extractVector2f(packet);
		
		sf::Sprite res;
		res.setTexture(resources.getTexture(textureName));
		res.setTextureRect(textureRect);
		res.setColor(color);
		res.setPosition(position);
		res.setRotation(rotation);
		res.setScale(scale);
		res.setOrigin(origin);
		return res;
	}
	
	shared_ptr<EntityManager> PacketFactory::extractEntities(sf::Packet &packet, ResourceManager &resources)
	{
		shared_ptr<EntityManager> entities;
		int type;
		packet >> type;
		sf::Vector2u windowSize=extractVector2u(packet);
		entities=make_shared<EntityManager>(windowSize);
		
		//For each entity: Extract components, Push components onto entity, Push entity onto entities
		while(packet >> type)
		{
			switch(type)
			{
				case ContentType::MEntity:
				{
					extractEntity(packet, entities, resources);
					break;
				}
				default:
				{
					break;
				}
			}
		}
		return entities;
	}
	
	shared_ptr<Entity> PacketFactory::extractEntity(sf::Packet &packet, shared_ptr<EntityManager> entities, ResourceManager &resources, int extractionId)
	{
		shared_ptr<Entity> entity;
		int id;
		packet >> id;
		int entityType;
		packet >> entityType;
		int playerId;
		packet >> playerId;
		
		if(id<entities->size() && (entity=entities->get(id))) //Change
		{
		
		}
		else //Add
		{
			switch(entityType)
			{
				case EntityType::ESprite:
				{
					entity=make_shared<Sprite>(entities);
					break;
				}
				default:
				{
					break;
				}
			}
			entity->setId(id);
			entities->insertAtId(entity);
		}
		entity->setPlayerId(playerId);
		
		//Components and attributes
		int type;
		while(packet >> type && type!=ContentType::MEntityEnd)
		{
			switch(type)
			{
				case ContentType::MComponent:
				{
					extractComponent(packet, entity, extractionId);
					break;
				}
				case ContentType::MEntityAttributes:
				{
					entity->extract(packet, resources, extractionId);
					break;
				}
				default:
				{
					break;
				}
			}
		}
		return entity;
	}
	
	shared_ptr<Component> PacketFactory::extractComponent(sf::Packet &packet, shared_ptr<Entity> entity, int extractionId)
	{
		shared_ptr<Component> component;
		int id;
		packet >> id;
		int componentType;
		packet >> componentType;
		
		if(id<entity->size() && (component=entity->get(id))) //Change
		{
		
		}
		else //Add
		{
			switch(componentType)
			{
				case ComponentType::CNone:
				{
					component=make_shared<Component>(entity, (ComponentType)componentType);
					break;
				}
				case ComponentType::CMovement:
				{
					component=make_shared<Movement>(entity);
				}
				default:
				{
					break;
				}
			}
			component->setId(id);
			entity->insertAtId(component);
		}
		
		component->extract(packet, extractionId);
		return component;
	}
}