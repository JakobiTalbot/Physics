#include "Sphere.h"
#include "AABB.h"

Sphere::Sphere(glm::vec2 v2Position, glm::vec2 v2Velocity, float fMass, float fRadius, float fElasticity, glm::vec4 v4Colour)
	: Rigidbody(SPHERE, v2Position, v2Velocity, 0, fMass, fElasticity)
{
	m_fRadius = fRadius;
	m_v4Colour = v4Colour;
}

Sphere::~Sphere()
{
}

void Sphere::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_v2Position, m_fRadius, 32, m_v4Colour);
}