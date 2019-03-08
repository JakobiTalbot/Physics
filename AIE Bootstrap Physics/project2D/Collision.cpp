#include "Collision.h"

Collision::Collision(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	m_pObjects.push_back(pObject1);
	m_pObjects.push_back(pObject2);
}

Collision::~Collision()
{
}