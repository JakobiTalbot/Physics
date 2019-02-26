#pragma once
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <vector>
#include "PhysicsObject.h"

class Rigidbody;

enum class ElasticityMode
{
	min,
	max,
	avg
};

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

	void CheckForCollision();

	static bool Plane2Plane(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool Plane2Sphere(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool Plane2AABB(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool Sphere2Plane(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool Sphere2Sphere(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool Sphere2AABB(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool AABB2Plane(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool AABB2Sphere(PhysicsObject* pObject1, PhysicsObject* pObject2);
	static bool AABB2AABB(PhysicsObject* pObject1, PhysicsObject* pObject2);

	static float CalculateElasticity(Rigidbody* pRigidbody1, Rigidbody* pRigidbody2, ElasticityMode elasticityMode);

protected:
	glm::vec2 m_v2Gravity;
	float m_fTimeStep;
	std::vector<PhysicsObject*> m_pActors;
	PhysicsObject* m_pSelectedObject;
	float m_fDebugTimeStep;
};