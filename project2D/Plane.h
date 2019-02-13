#pragma once
#include "Rigidbody.h"
class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 v2Normal, float fDistance);
	~Plane();

	virtual void FixedUpdate(glm::vec2 fGravity, float fTimeStep);
	virtual void Debug();
	virtual void MakeGizmo();
	virtual void ResetPosition();

	glm::vec2 GetNormal() { return m_v2Normal; }
	float GetDistance() { return m_fDistanceToOrigin; }

protected:
	glm::vec2 m_v2Normal;
	float m_fDistanceToOrigin;
};