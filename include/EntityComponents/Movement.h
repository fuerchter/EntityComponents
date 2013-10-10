#ifndef MOVEMENT
#define MOVEMENT

#include <map>
using namespace std;

#include "Component.h"
#include "PacketFactory.h"

namespace ec
{
	class Movement : public Component
	{
	public:
		Movement(shared_ptr<Entity> entity);
		
		virtual void update(float deltaTime, int playerId) override;
		
		virtual sf::Packet &append(sf::Packet &packet, int id=-1) override;
		virtual sf::Packet &extract(sf::Packet &packet, int id=-1) override;
	private:
		int sequence_;
		map<int, sf::Vector2f> input_;
		map<int, sf::Vector2f> predicted_;
		map<int, sf::Vector2f> actual_;
	};
}

#endif