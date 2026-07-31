#include "entity.hpp"
#include "game.hpp"
#include "component.hpp"
#include <algorithm>

Entity::Entity(Game* game):
	mState(EActive),
	mPosition(Vector2::Zero),
	mScale(1.0f),
	mRotation(0.0f),
	mGame(game)
{
	mGame->AddEntity(this);
}

Entity::~Entity()
{
	mGame->RemoveEntity(this);
	// Need to delete components
	// Because ~Component calls RemoveComponent, need a different style loop
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Entity::Update(float deltaTime)
{
	if (mState == EActive)
	{
		UpdateComponents(deltaTime);
		UpdateEntity(deltaTime);
	}
}

void Entity::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Entity::UpdateEntity(float deltaTime)
{

}

void Entity::AddComponent(Component* component)
{
	// Find the insertion point in the sorted vector
	// the first element with a order higher than me
	int myOrder = component->GetUpdateOrder();
	// auto iter = mComponents.begin();
	for (auto iter = mComponents.begin(); iter!= mComponents.end(); ++iter)
	{
		if (mOrder < (*iter)->GetUpdateOrder())
		{
			break; 
		}	
	}

	// insert element before position of iterator
	mComponents.insert(iter, component);
}

void Entity::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
