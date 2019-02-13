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