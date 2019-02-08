#include "Rigidbody.h"



Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 v2Position, glm::vec2 v2Velocity, float fRotation, float fMass)
{
	m_shapeID = shapeID;
	m_v2Position = v2Position;
	m_v2Velocity = v2Velocity;
	m_fRotation = fRotation;
	m_fMass = fMass;
}


Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 v2Gravity, float fTimeStep)
{
	ApplyForce(v2Gravity * m_fMass * fTimeStep);
	m_v2Position += m_v2Velocity * fTimeStep;
}

void Rigidbody::Debug()
{

}

void Rigidbody::ApplyForce(glm::vec2 v2Force)
{
	m_v2Velocity += v2Force / m_fMass;
}

void Rigidbody::ApplyForceToActor(Rigidbody* pActor, glm::vec2 v2Force)
{
	// get direction between rigidbodies
	glm::vec2 v2Dir = pActor->GetPosition() - this->GetPosition();
	v2Dir = glm::normalize(v2Dir);

	float fMagnitude = v2Force.length();
	float fTotalMass = pActor->GetMass() + this->GetMass();

	// apply forces to rigidbodies
	pActor->ApplyForce((v2Dir * fMagnitude * (pActor->GetMass()/ fTotalMass)));
	ApplyForce((-v2Dir * fMagnitude * (this->GetMass() / fTotalMass)));
}