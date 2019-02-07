#pragma once
#include <Gizmos.h>
#include <glm\ext.hpp>

enum ShapeType {
	PLANE = 0,
	SPHERE,
	BOX
};

class PhysicsObject
{
protected:
	PhysicsObject() {}
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	virtual void FixedUpdate(glm::vec2 v2Gravity, float fTimeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	virtual void ResetPosition() {}

protected:
	ShapeType m_shapeID;
};