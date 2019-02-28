#pragma once
#include "Rigidbody.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 v2Position, glm::vec2 v2Velocity, float fMass, float fRadius, float fElasticity, glm::vec4 v4Colour);
	~Sphere();

	virtual void MakeGizmo();

	float GetRadius() { return m_fRadius; }
	glm::vec4 GetColour() { return m_v4Colour; }

protected:
	float m_fRadius;
	glm::vec4 m_v4Colour;
};