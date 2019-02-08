#pragma once
#include "Rigidbody.h"
class AABB : public Rigidbody
{
public:
	AABB(glm::vec2 v2Position, glm::vec2 v2Velocity, float fRotation, float fMass, glm::vec2 v2Extent, glm::vec4 v4Colour);
	~AABB();

	virtual void MakeGizmo();
	virtual bool CheckCollision(PhysicsObject* pOther);

	glm::vec2 GetMaximum() { return m_v2Maximum; }
	glm::vec2 GetMinimum() { return m_v2Minimum; }

protected:
	glm::vec2 m_v2Minimum;
	glm::vec2 m_v2Maximum;
	glm::vec4 m_v4Colour;
};