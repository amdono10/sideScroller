#include "component.hpp"
#include "entity.hpp"

Component::Component(Entity* owner, int updateOrder):
	mOwner(owner),
	mUpdateOrder(updateOrder)
{
	// Add to entities vector of components
	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
}

// virtual function to be overwritten
void Component::Update(float deltaTime)
{

}