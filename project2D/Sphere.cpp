#include "Sphere.h"

Sphere::Sphere(glm::vec2 v2Position, glm::vec2 v2Velocity, float fMass, float fRadius, glm::vec4 v4Colour)
	: Rigidbody(SPHERE, v2Position, v2Velocity, 0, fMass)
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

bool Sphere::CheckCollision(PhysicsObject* pOther)
{
	// check if other PhysicsObject is a sphere
	Sphere* pOtherSphere = dynamic_cast<Sphere*>(pOther);
	if (!pOtherSphere)
		return false;

	// check collision
	if (glm::distance(m_v2Position, pOtherSphere->GetPosition()) < (m_fRadius + pOtherSphere->GetRadius()))
	{
		return true;
	}

	return false;
}