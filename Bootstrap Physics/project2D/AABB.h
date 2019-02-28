#pragma once
#include "Rigidbody.h"
class AABB : public Rigidbody
{
public:
	AABB(glm::vec2 v2Position, glm::vec2 v2Velocity, float fRotation, float fMass, glm::vec2 v2Extent, float fElasticity, glm::vec4 v4Colour);
	~AABB();

	virtual void MakeGizmo();

	glm::vec2 GetExtent() { return m_v2Extent; }

protected:
	glm::vec2 m_v2Extent;
	glm::vec4 m_v4Colour;
};