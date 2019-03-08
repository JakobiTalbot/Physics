#include "Plane.h"

Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_fDistanceToOrigin = 0;
	m_v2Normal = glm::vec2(0, 1);
}

Plane::Plane(glm::vec2 v2Normal, float fDistance) : PhysicsObject(ShapeType::PLANE)
{
	m_v2Normal = v2Normal;
	m_fDistanceToOrigin = fDistance;
}

Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 fGravity, float fTimeStep)
{

}

void Plane::Debug()
{

}

void Plane::MakeGizmo()
{
	float fLineSegmentLength = 300.f;
	glm::vec2 v2CentrePoint = m_v2Normal * m_fDistanceToOrigin;
	glm::vec2 v2Parallel(m_v2Normal.y, -m_v2Normal.x);

	glm::vec2 v2Start = v2CentrePoint + (v2Parallel * fLineSegmentLength);
	glm::vec2 v2End = v2CentrePoint - (v2Parallel * fLineSegmentLength);
	aie::Gizmos::add2DLine(v2Start, v2End, { 1, 1, 1, 1 });
}

void Plane::ResetPosition()
{

}