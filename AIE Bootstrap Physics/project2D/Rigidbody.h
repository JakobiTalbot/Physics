#pragma once
#include "PhysicsObject.h"
class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 v2Position, glm::vec2 v2Velocity, float fRotation, float fMass, float fElasticity);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 v2Gravity, float fTimeStep);
	virtual void Debug();
	void ApplyForce(glm::vec2 v2Force);
	void ApplyForceToActor(Rigidbody* pActor, glm::vec2 v2Force);

	glm::vec2 GetPosition() { return m_v2Position; }
	void SetPosition(glm::vec2 v2Position) { m_v2Position = v2Position; }
	glm::vec2 GetVelocity() { return m_v2Velocity; }
	void SetVelocity(glm::vec2 v2Velocity) { m_v2Velocity = v2Velocity; }
	float GetMass() { return m_fMass; }
	float GetRotation() { return m_fRotation; }
	float GetElasticity() { return m_fElasticity; }

protected:
	glm::vec2 m_v2Position;
	glm::vec2 m_v2Velocity;
	float m_fMass;
	float m_fRotation;
	float m_fElasticity;
};

