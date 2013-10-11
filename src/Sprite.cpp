#include "EntityComponents/Sprite.h"

namespace ec
{
	Sprite::Sprite(shared_ptr<EntityManager> manager):
	Entity(manager, EntityType::ESprite)
	{
		transformable=make_shared<sf::Sprite>();
	}

	sf::FloatRect Sprite::getBounds()
	{
		return (*getSprite()).getGlobalBounds();
	}
	
	void Sprite::draw(sf::RenderWindow &window)
	{
		if(transformable)
		{
			window.draw(*getSprite());
		}
	}
	
	sf::Packet &Sprite::append(sf::Packet &packet, int id)
	{
		packet << (int)ContentType::MEntityAttributes;
		PacketFactory::append(packet, *getSprite(), textureName, false);
		return packet;
	}
	
	sf::Packet &Sprite::extract(sf::Packet &packet, ResourceManager &resources, int id)
	{
		packet >> textureName;
		shared_ptr<sf::Sprite> sprite=make_shared<sf::Sprite>(PacketFactory::extractSprite(packet, textureName, resources));
		if(!predicting_)
		{
			transformable=static_pointer_cast<sf::Transformable>(sprite);
		}
		return packet;
	}
	
	shared_ptr<sf::Sprite> Sprite::getSprite()
	{
		return static_pointer_cast<sf::Sprite>(transformable);
	}
}