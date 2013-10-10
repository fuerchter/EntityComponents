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
		//Better id distribution (factory function?)!
		//Do Components have to be marked for deletion?
		
		//Get rid of manager append/extract
		//ComponentType has to be accessible to library user (therefore sfec has to be compiled with ComponentType folder included?).
		EntityManager(sf::Vector2u windowSize);
		void update(float deltaTime, int playerId=-1);
		void draw(sf::RenderWindow &window);
		
		void pushBack(shared_ptr<Entity> entity);
		void insertAtId(shared_ptr<Entity> entity); //Replaces the current entity at the new entity's id
		int size();
		shared_ptr<Entity> get(int id); //TODO: Check whether id>=size()
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