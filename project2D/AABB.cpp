#include "AABB.h"

AABB::AABB(glm::vec2 v2Position, glm::vec2 v2Velocity, float fRotation, float fMass, glm::vec2 v2Extent, glm::vec4 v4Colour)
	: Rigidbody(BOX, v2Position, v2Velocity, fRotation, fMass)
{
	m_v2Maximum = v2Extent;
	m_v2Minimum = -v2Extent;
	m_v4Colour = v4Colour;
}

AABB::~AABB()
{
}

void AABB::MakeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_v2Position, m_v2Maximum, m_v4Colour);
}

bool AABB::CheckCollision(PhysicsObject* pOther)
{
	switch (pOther->GetShapeID())
	{
	case SPHERE:
		// check collision
		//Sphere* pOtherSphere = (Sphere*)pOther;
		//if (glm::distance(m_v2Position, pOtherSphere->GetPosition()) < (m_fRadius + pOtherSphere->GetRadius()))
		//{
		//	return true;
		//}
		break;
	case BOX:
		AABB* pOtherBox = (AABB*)pOther;
		if (m_v2Minimum.x < pOtherBox->GetMaximum().x
			&& m_v2Maximum.x > pOtherBox->GetMinimum().y
			&& m_v2Minimum.y < pOtherBox->GetMaximum().y
			&& m_v2Maximum.y > pOtherBox->GetMinimum().y)
			return true;
		break;
	}

	return false;
}