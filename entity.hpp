#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include <vector>
#include "math.hpp"

class Entity {
public:
	enum State {
		EActive,
		EPaused,
		EDead
	};

	Entity(class Game* game);
	virtual ~Entity();

	// Update function called from Game (not overrideable)
	void Update(float deltaTime);
	// Updates all the components attached to the entity (not overridable)
	void UpdateComponents(float deltaTime);
	// Any entity-specific update code (overridable)
	virtual void UpdateEntity(float deltaTime);

	// getters & setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// add & remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// Entity's state
	State mState;
	
	// Transform
	Vector2 mPosition;
	float mScale;
	float mRotation;

	// std::vector to store components of entity
	std::vector<class Component*> mComponents;
	class Game* mGame;
};

#endif	// _ENTITY_HPP