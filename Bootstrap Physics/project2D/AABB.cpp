#include "AABB.h"

AABB::AABB(glm::vec2 v2Position, glm::vec2 v2Velocity, float fRotation, float fMass, glm::vec2 v2Extent, float fElasticity, glm::vec4 v4Colour)
	: Rigidbody(BOX, v2Position, v2Velocity, fRotation, fMass, fElasticity)
{
	m_v2Extent = v2Extent;
	m_v4Colour = v4Colour;
}

AABB::~AABB()
{
}

void AABB::MakeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_v2Position, m_v2Extent, { 0, 0, 0, 1 });
	aie::Gizmos::add2DAABBFilled(m_v2Position, m_v2Extent * 0.9f, m_v4Colour);
}