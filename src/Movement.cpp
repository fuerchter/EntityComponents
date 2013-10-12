#include "EntityComponents/Movement.h"

namespace ec
{
	Movement::Movement(shared_ptr<Entity> entity):
	Component(entity, ComponentType::CMovement), sequence_(0)
	{
	
	}

	void Movement::update(float deltaTime, sf::RenderWindow &window, int playerId)
	{
		shared_ptr<Entity> entity=entity_.lock();
		//sf::Vector2f position=entity->transformable->getPosition();
		//cout << position.x << " " << position.y << endl;
		if(playerId==-1)
		{
			for(auto input : input_)
			{
				sf::Vector2f actual=input.second*40.0f*deltaTime;
				actual_.insert(pair<int, sf::Vector2f>(input.first, actual));
				entity->transformable->move(actual);
			}
			input_.clear();
		}
		else if(playerId==entity->getPlayerId())
		{
			for(auto actual : actual_)
			{
				//Compare actual to predicted
				if(predicted_[actual.first]!=actual.second)
				{
					entity->transformable->move(actual.second-predicted_[actual.first]);
					//cout << "Did not predict properly!" << endl;
				}
				else
				{
					//cout << "Proper prediction, yay!" << endl;
				}
				predicted_.erase(actual.first);
			}
			actual_.clear();
		
			sf::Vector2f velocity;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				velocity.y--;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				velocity.y++;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				velocity.x--;
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				velocity.x++;
			}
			
			float length=sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
			if(length!=0)
			{
				velocity/=length;
				//Record input and predicted in collection
				input_.insert(pair<int, sf::Vector2f>(sequence_, velocity));
				
				sf::Vector2f predicted=velocity*40.0f*deltaTime;
				predicted_.insert(pair<int, sf::Vector2f>(sequence_, predicted));
				sequence_++;
				entity->transformable->move(predicted);
			}
		}
	}
	
	void Movement::onCollision(shared_ptr<Entity> collider, int playerId)
	{
		shared_ptr<Entity> entity=entity_.lock();
		if(playerId==-1)
		{
			//Server collision reaction (important!)
			/*if(!actual_.empty())
			{
				sf::FloatRect firstBounds=entity->getBounds();
				sf::FloatRect secondBounds=collider->getBounds();
				
				int counter=0;
				for(auto it=actual_.rbegin(); firstBounds.intersects(secondBounds) && it!=actual_.rend(); it++)
				{
					entity->transformable->move(-it->second);
					it->second.x=0;
					it->second.y=0;
					firstBounds=entity->getBounds();
					counter++;
				}
				//cout << "Iterated " << counter << " times, input size: " << actual_.size() << endl;
			}*/
		}
		else if(playerId==entity->getPlayerId())
		{
			//Client collision reaction
			for(auto input : input_)
			{
				entity->transformable->move(-predicted_[input.first]);
				predicted_.erase(input.first);
			}
			input_.clear();
		}
	}
	
	sf::Packet &Movement::append(sf::Packet &packet, int id)
	{
		if(id==-1)
		{
			packet << (sf::Uint32)actual_.size();
			for(auto actual : actual_)
			{
				packet << actual.first;
				PacketFactory::append(packet, actual.second, false);
			}
			//Clear actual on send
			actual_.clear();
		}
		else
		{
			packet << (sf::Uint32)input_.size();
			for(auto input : input_)
			{
				packet << input.first;
				PacketFactory::append(packet, input.second, false);
			}
			//Clear input on send
			input_.clear();
		}
		return packet;
	}

	sf::Packet &Movement::extract(sf::Packet &packet, int id)
	{
		if(id==-1)
		{
			int size;
			packet >> size;
			for(int i=0; i<size; i++)
			{
				int sequence;
				packet >> sequence;
				sf::Vector2f input=PacketFactory::extractVector2f(packet);
				input_.insert(pair<int, sf::Vector2f>(sequence, input));
			}
		}
		else
		{
			int size;
			packet >> size;
			for(int i=0; i<size; i++)
			{
				int sequence;
				packet >> sequence;
				sf::Vector2f actual=PacketFactory::extractVector2f(packet);
				actual_.insert(pair<int, sf::Vector2f>(sequence, actual));
			}
		}
		return packet;
	}
}