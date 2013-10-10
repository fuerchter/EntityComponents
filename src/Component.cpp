#include "EntityComponents/Component.h"

namespace ec
{
	Component::Component(shared_ptr<Entity> entity, ComponentType type)
	{
		entity_=weak_ptr<Entity>(entity);
		type_=type;
	}

	ComponentType Component::getType()
	{
		return type_;
	}
	
	int Component::getId()
	{
		return id_;
	}

	void Component::setId(int id)
	{
		id_=id;
	}
}