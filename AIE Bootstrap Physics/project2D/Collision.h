#pragma once
#include "PhysicsObject.h"
#include <vector>
class Collision
{
public:
	Collision(PhysicsObject* pObject1, PhysicsObject* pObject2);
	~Collision();

protected:
	std::vector<PhysicsObject*> m_pObjects;
};