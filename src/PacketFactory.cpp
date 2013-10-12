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

	void PacketFactory::append(sf::Packet &packet, sf::IntRect content, bool header)
	{
		if(header)
		{
			packet << (int)MessageType::MIntRect;
		}
		
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

	void PacketFactory::append(sf::Packet &packet, sf::Color content, bool header)
	{
		if(header)
		{
			packet << (int)MessageType::MColor;
		}
		
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

	void PacketFactory::append(sf::Packet &packet, sf::Vector2f content, bool header)
	{
		if(header)
		{
			packet << (int)MessageType::MVector2f;
		}
		
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
	
	void PacketFactory::append(sf::Packet &packet, sf::Vector2u content, bool header)
	{
		if(header)
		{
			packet << (int)MessageType::MVector2u;
		}
		
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

	void PacketFactory::append(sf::Packet &packet, sf::Sprite sprite, string textureName, bool header)
	{
		if(header)
		{
			packet << (int)MessageType::MSprite;
		}
		
		packet << textureName;
		append(packet, sprite.getTextureRect(), false);
		append(packet, sprite.getColor(), false);
		
		append(packet, sprite.getPosition(), false);
		packet << sprite.getRotation();
		append(packet, sprite.getScale(), false);
		append(packet, sprite.getOrigin(), false);
	}

	sf::Sprite PacketFactory::extractSprite(sf::Packet &packet, string textureName, ResourceManager &resources)
	{
		sf::IntRect textureRect=extractIntRect(packet);
		sf::Color color=extractColor(packet);
		sf::Vector2f position=extractVector2f(packet);
		float rotation;
		packet >> rotation;
		sf::Vector2f scale=extractVector2f(packet);
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
		while(packet >> type && type!=MessageType::MEntityEnd)
		{
			switch(type)
			{
				case MessageType::MComponent:
				{
					extractComponent(packet, entity, extractionId);
					break;
				}
				case MessageType::MEntityAttributes:
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