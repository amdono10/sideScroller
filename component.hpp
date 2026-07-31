#ifndef _COMPONENT_HPP
#define _COMPONENT_HPP

class Component {
public:
	// Constructor
	// the lower the update order, the earlier the component updates
	Component(class Entity* owner, int updateOrder = 100);
	// Destructor
	virtual ~Component();
	// Update this component by delta time
	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// Owning Entity
	class Entity* mOwner;
	// Update order of component
	int mUpdateOrder;
};


#endif // _COMPONENT_HPP