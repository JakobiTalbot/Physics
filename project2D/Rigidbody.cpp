#include "Rigidbody.h"



Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 v2Position, glm::vec2 v2Velocity, float fRotation, float fMass, float fElasticity)
{
	m_shapeID = shapeID;
	m_v2Position = v2Position;
	m_v2Velocity = v2Velocity;
	m_fRotation = fRotation;
	m_fMass = fMass;
	m_fElasticity = fElasticity;
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
	// calculate impulse
	float fImpulse = -(1.f + GetElasticity()) * glm::dot(v2Dir, glm::normalize(v2Dir));
	fImpulse /= glm::dot(glm::normalize(v2Dir), glm::normalize(v2Dir) * (m_fMass + pActor->GetMass()));

	float fTotalMass = pActor->GetMass() + this->GetMass();

	// apply forces to rigidbodies
	pActor->ApplyForce((-glm::normalize(v2Dir) * fImpulse * (pActor->GetMass()/ fTotalMass)));
	ApplyForce((glm::normalize(v2Dir) * fImpulse * (this->GetMass() / fTotalMass)));
}