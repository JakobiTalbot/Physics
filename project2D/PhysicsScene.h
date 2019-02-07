#pragma once
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <vector>
#include "PhysicsObject.h"
class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor);
	void DebugScene();
	void Update(float fDeltaTime);
	void UpdateGizmos();

	void SetGravity(const glm::vec2 v2Gravity) { m_v2Gravity = v2Gravity; }
	glm::vec2 GetGravity() const { return m_v2Gravity; }

	void SetTimeStep(const float fTimeStep) { m_fTimeStep = fTimeStep; }
	float GetTimeStep() const { return m_fTimeStep; }

protected:
	glm::vec2 m_v2Gravity;
	float m_fTimeStep;
	std::vector<PhysicsObject*> m_pActors;
};