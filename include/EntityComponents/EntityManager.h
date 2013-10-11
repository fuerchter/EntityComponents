#ifndef ENTITYMANAGER
#define ENTITYMANAGER

#include <iostream>

#include <memory>
#include <vector>
using namespace std;

#include "SFML/Graphics.hpp"

#include "Entity.h"

namespace ec
{
	class Entity;

	class EntityManager
	{
	public:
		EntityManager(sf::Vector2u windowSize);
		void update(float deltaTime, int playerId=-1);
		void draw(sf::RenderWindow &window);
		
		void pushBack(shared_ptr<Entity> entity);
		void insertAtId(shared_ptr<Entity> entity); //Replaces the current entity at the new entity's id
		int size();
		shared_ptr<Entity> get(int id);
		vector<shared_ptr<Entity>> get(EntityType type); //Returns all entities matching the given type
		vector<int> getPlayer(int id);
		void remove(int id);
		
		sf::Vector2u getWindowSize();
		
		sf::Packet& operator>>(sf::Packet& packet);
	private:
		void updateEntities(float deltaTime, int playerId=-1);
		void removeEntities();
		
		vector<shared_ptr<Entity>> entities_;
		vector<int> marked_;
		
		sf::Vector2u windowSize_;
	};
}

#endif