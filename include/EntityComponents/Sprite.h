#ifndef SPRITE
#define SPRITE

#include "Entity.h"
#include "PacketFactory.h"

namespace ec
{
	class EntityManager;

	class Sprite : public Entity
	{
	public:
		Sprite(shared_ptr<EntityManager> manager);
		virtual sf::FloatRect getBounds() override;
		virtual void draw(sf::RenderWindow &window) override;
		
		virtual sf::Packet &append(sf::Packet &packet, int id=-1) override;
		virtual sf::Packet& extract(sf::Packet& packet, ResourceManager &resources, int id=-1) override;
		
		shared_ptr<sf::Sprite> getSprite();
		
		string textureName;
	private:
	};
}

#endif