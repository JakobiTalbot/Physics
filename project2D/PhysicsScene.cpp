#include "PhysicsScene.h"
#include "Rigidbody.h"
#include "Application2D.h"
#include "Collision.h"
#include "Hash.h"
#include "Input.h"
#include <list>
#include <iostream>
#include "Plane.h"
#include "AABB.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
static fn CollisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane,		PhysicsScene::Plane2Sphere,		PhysicsScene::Plane2AABB,
	PhysicsScene::Sphere2Plane,		PhysicsScene::Sphere2Sphere,	PhysicsScene::Sphere2AABB,
	PhysicsScene::AABB2Plane,		PhysicsScene::AABB2Sphere,		PhysicsScene::AABB2AABB
};

PhysicsScene::PhysicsScene()
{
	// initialise variables
	m_fTimeStep = 0.01f;
	m_v2Gravity = glm::vec2(0, 0);
	m_pSelectedObject = nullptr;
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_pActors)
		delete pActor;
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	// add actor to back of vector
	m_pActors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	// find index of actor to be removed
	for (int i = 0; i < m_pActors.size(); ++i)
	{
		if (m_pActors[i] == actor)
		{
			// erase actor
			m_pActors.erase(m_pActors.begin() + i);
		}
	}
}

void PhysicsScene::DebugScene()
{
	// activate all debugs
	int nCount = 0;
	for (auto pActor : m_pActors)
	{
		std::cout << nCount << " : ";
		pActor->Debug();
		++nCount;
	}
}

void PhysicsScene::Update(float fDeltaTime)
{
	// list of already collided PhysicsObjects
	static std::list<Collision*> collisions;

	// update physics at a fixed time step
	static float fAccumulatedTime = 0.f;
	fAccumulatedTime += fDeltaTime;

	float fAspectRatio = (float)Application2D::GetInstance()->getWindowHeight() / (float)Application2D::GetInstance()->getWindowWidth();
	glm::vec2 v2MousePos = glm::vec2(aie::Input::getInstance()->getMouseX() / (float)Application2D::GetInstance()->getWindowWidth(), aie::Input::getInstance()->getMouseY() / (float)Application2D::GetInstance()->getWindowHeight());
	v2MousePos.x *= 200.0f;
	v2MousePos.x -= 100.0f;
	v2MousePos.y *= 112.5f;
	v2MousePos.y -= 56.25f;

	// DRAG OBJECT
	if (!m_pSelectedObject)
	{
		if (aie::Input::getInstance()->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
		{
			for (auto pActors : m_pActors)
			{
				// get shape type pointers
				Sphere* pSphere = (Sphere*)pActors;
				AABB* pAABB = (AABB*)pActors;

				// find if mouse is on shape, depending on shape type
				switch (pActors->GetShapeID())
				{
				case ShapeType::SPHERE:
					if (glm::distance(v2MousePos, pSphere->GetPosition()) <= pSphere->GetRadius())
					{
						m_pSelectedObject = pActors;
						break;
					}
					break;
				case ShapeType::BOX:
					if (v2MousePos.x < pAABB->GetPosition().x + pAABB->GetExtent().x
						&& v2MousePos.y < pAABB->GetPosition().y + pAABB->GetExtent().y
						&& v2MousePos.x > pAABB->GetPosition().x - pAABB->GetExtent().x
						&& v2MousePos.y > pAABB->GetPosition().y - pAABB->GetExtent().y)
					{
						m_pSelectedObject = pActors;
						break;
					}
					break;
				}
			}
		}
	}
	else
	{
		Rigidbody* rb = (Rigidbody*)m_pSelectedObject;
		rb->ApplyForce((v2MousePos - rb->GetPosition()) * 0.2f);
		aie::Gizmos::add2DLine(v2MousePos, rb->GetPosition(), glm::vec4(1,0,1,1));

		if (aie::Input::getInstance()->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			m_pSelectedObject = nullptr;
	}

	// fixed update
	while (fAccumulatedTime >= m_fTimeStep)
	{
		fAccumulatedTime -= m_fTimeStep;

		// check for collisions
		for (auto pActor : m_pActors)
		{
			pActor->FixedUpdate(m_v2Gravity, m_fTimeStep);
			Rigidbody* pRigid = (Rigidbody*)pActor;

			CheckForCollision();
			float fAspectRatio = (float)Application2D::GetInstance()->getWindowHeight() / (float)Application2D::GetInstance()->getWindowWidth();
		}

		// clear collision list
		for (auto collision : collisions)
		{
			delete collision;
			collision = nullptr;
		}
		collisions.clear();
	}
}

void PhysicsScene::UpdateGizmos()
{
	// add gizmos for each actor
	for (auto pActor : m_pActors)
	{
		pActor->MakeGizmo();
	}
}

void PhysicsScene::CheckForCollision()
{
	for (int i = 0; i < m_pActors.size() - 1; ++i)
	{
		for (int j = i + 1; j < m_pActors.size(); ++j)
		{
			PhysicsObject* pObject1 = m_pActors[i];
			PhysicsObject* pObject2 = m_pActors[j];
			int nShapeID1 = pObject1->GetShapeID();
			int nShapeID2 = pObject2->GetShapeID();

			// function pointers
			int functionIdx = (nShapeID1 * SHAPETYPECOUNT) + nShapeID2;
			fn collisionFunctionPtr = CollisionFunctionArray[functionIdx];

			if (collisionFunctionPtr)
			{
				// check for collision
				collisionFunctionPtr(pObject1, pObject2);
			}
		}
	}
}

bool PhysicsScene::Plane2Plane(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	Plane* pPlane1 = (Plane*)pObject1;
	Plane* pPlane2 = (Plane*)pObject2;
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	Plane* pPlane = (Plane*)pObject1;
	Sphere* pSphere = (Sphere*)pObject2;

	glm::vec2 v2CollisionNormal = pPlane->GetNormal();
	float fSphereToPlane = glm::dot(pSphere->GetPosition(), pPlane->GetNormal()) - pPlane->GetDistance();

	// flip normal if sphere is behind plane
	if (fSphereToPlane < 0)
	{
		v2CollisionNormal *= -1.f;
		fSphereToPlane *= -1.f;
	}

	float fIntersection = pSphere->GetRadius() - fSphereToPlane;
	if (fIntersection > 0)
	{
		// objects collide
		pSphere->SetPosition(pSphere->GetPosition() + v2CollisionNormal * fIntersection);
		pSphere->ApplyForce(-(1 + pSphere->GetElasticity()) * glm::dot(pSphere->GetVelocity(), v2CollisionNormal) * v2CollisionNormal);
		return true;
	}
	return false;
}

bool PhysicsScene::Plane2AABB(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	Plane* pPlane = (Plane*)pObject1;
	AABB* pAABB = (AABB*)pObject2;

	glm::vec2 v2CollisionNormal = pPlane->GetNormal();
	float fAABBToPlane = glm::dot(pAABB->GetPosition(), pPlane->GetNormal()) - pPlane->GetDistance();

	// flip normal if AABB is behind plane
	if (fAABBToPlane < 0)
	{
		v2CollisionNormal *= -1.f;
		fAABBToPlane *= -1.f;
	}

	float fIntersection = glm::dot(pAABB->GetExtent() * v2CollisionNormal, v2CollisionNormal) - fAABBToPlane;
	if (fIntersection > 0)
	{
		//objects collide
		pAABB->SetPosition(pAABB->GetPosition() + v2CollisionNormal * fIntersection);
		pAABB->ApplyForce((-(1 + pAABB->GetElasticity()) * glm::dot(pAABB->GetVelocity(), v2CollisionNormal) * v2CollisionNormal));
		return true;
	}

	return false;
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	// inverse the pointers and call Plane2Sphere
	return Plane2Sphere(pObject2, pObject1);
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	Sphere* pSphere1 = (Sphere*)pObject1;
	Sphere* pSphere2 = (Sphere*)pObject2;
	// check if distance is lower than sum of radii
	if (glm::distance(pSphere1->GetPosition(), pSphere2->GetPosition()) < (pSphere1->GetRadius() + pSphere2->GetRadius()))
	{
		// restitution
		float fPenetration = glm::distance(pSphere1->GetPosition(), pSphere2->GetPosition()) - (pSphere1->GetRadius() + pSphere2->GetRadius());

		// get direction between rigidbodies
		glm::vec2 v2Dir = pSphere1->GetPosition() - pSphere2->GetPosition();

		// calculate impulse
		float fImpulse = -(1.f + pSphere2->GetElasticity()) * glm::dot((pSphere2->GetVelocity() - pSphere1->GetVelocity()), glm::normalize(v2Dir));
		fImpulse /= glm::dot(glm::normalize(v2Dir), glm::normalize(v2Dir) * (1 / pSphere2->GetMass() + 1 / pSphere1->GetMass()));

		float fTotalMass = pSphere1->GetMass() + pSphere2->GetMass();
		pSphere1->SetPosition(pSphere1->GetPosition() + -glm::normalize(v2Dir) * (fPenetration / 2.f));
		pSphere2->SetPosition(pSphere2->GetPosition() + glm::normalize(v2Dir) * (fPenetration / 2.f));

		// apply forces to rigidbodies
		pSphere1->ApplyForce((-glm::normalize(v2Dir) * fImpulse * (pSphere1->GetMass() / fTotalMass)));
		pSphere2->ApplyForce((glm::normalize(v2Dir) * fImpulse * (pSphere2->GetMass() / fTotalMass)));
		return true;
	}
	return false;
}

bool PhysicsScene::Sphere2AABB(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	Sphere* pSphere = (Sphere*)pObject1;
	AABB* pAABB = (AABB*)pObject2;

	// find closest point to sphere
	glm::vec2 v2ClosestPoint = glm::clamp(pSphere->GetPosition(), pAABB->GetPosition() - pAABB->GetExtent(), pAABB->GetPosition() + pAABB->GetExtent());

	// check if closest point is within radius
	if (glm::distance(v2ClosestPoint, pSphere->GetPosition()) < pSphere->GetRadius())
	{
		glm::vec2 v2CollisionNormal = pSphere->GetPosition() - v2ClosestPoint;
		v2CollisionNormal = glm::normalize(v2CollisionNormal);

		float fCollisionDepth = glm::distance(v2ClosestPoint, pSphere->GetPosition()) - pSphere->GetRadius();

		// calculate impulse
		float fImpulse = -(1.f + pAABB->GetElasticity()) * glm::dot((pAABB->GetVelocity() - pSphere->GetVelocity()), v2CollisionNormal);
		fImpulse /= glm::dot(v2CollisionNormal, v2CollisionNormal * (1 / pAABB->GetMass() + 1 / pSphere->GetMass()));

		float fTotalMass = pSphere->GetMass() + pAABB->GetMass();

		pSphere->SetPosition(pSphere->GetPosition() + -v2CollisionNormal * (fCollisionDepth / 2.f));
		pAABB->SetPosition(pAABB->GetPosition() + v2CollisionNormal * (fCollisionDepth / 2.f));

		// apply forces to rigidbodies
		pSphere->ApplyForce((-v2CollisionNormal * fImpulse * (pSphere->GetMass() / fTotalMass)));
		pAABB->ApplyForce((v2CollisionNormal * fImpulse * (pAABB->GetMass() / fTotalMass)));
		return true;
	}
	return false;
}

bool PhysicsScene::AABB2Plane(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	// inverse the pointers and call Plane2AABB
	return Plane2AABB(pObject2, pObject1);
}

bool PhysicsScene::AABB2Sphere(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	// inverse the pointers and call Sphere2AABB
	return Sphere2AABB(pObject2, pObject1);
}

bool PhysicsScene::AABB2AABB(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	AABB* pAABB1 = (AABB*)pObject1;
	AABB* pAABB2 = (AABB*)pObject2;

	// normals of each face
	static const glm::vec2 v2Faces[4] =
	{
		{-1, 0}, // left
		{1, 0}, // right
		{0, -1}, // bottom
		{0, 1} // top
	};

	// distances between faces
	float fDistances[4] =
	{
		(pAABB2->GetPosition().x + pAABB1->GetExtent().x - (pAABB1->GetPosition().x - pAABB2->GetExtent().x)), // left
		(pAABB1->GetPosition().x + pAABB1->GetExtent().x - (pAABB2->GetPosition().x - pAABB2->GetExtent().x)), // right
		(pAABB2->GetPosition().y + pAABB1->GetExtent().y - (pAABB1->GetPosition().y - pAABB2->GetExtent().y)), // bottom
		(pAABB1->GetPosition().y + pAABB1->GetExtent().y - (pAABB2->GetPosition().y - pAABB2->GetExtent().y)) // top
	};

	float fCollisionDepth = 0.0f;
	glm::vec2 v2CollisionNormal = { 0, 0 };
	int iCollisionFace = NULL;

	// find collision normal
	for (int i = 0; i < 4; ++i)
	{
		// box does not intersect face
		if (fDistances[i] < 0.0f)
			return false;
		else if (i == 0 || fDistances[i] < fCollisionDepth) // lower intersection depth than previously scanned faces
		{
			fCollisionDepth = fDistances[i];
			v2CollisionNormal = v2Faces[i];
			iCollisionFace = i;
		}
	}

	// calculate impulse
	float fImpulse = -(1.f + pAABB2->GetElasticity()) * glm::dot((pAABB2->GetVelocity() - pAABB1->GetVelocity()), v2CollisionNormal);
	fImpulse /= glm::dot(v2CollisionNormal, v2CollisionNormal * (1 / pAABB2->GetMass() + 1 / pAABB1->GetMass()));

	float fTotalMass = pAABB1->GetMass() + pAABB2->GetMass();
	pAABB1->SetPosition(pAABB1->GetPosition() + -v2CollisionNormal * (fCollisionDepth / 2.f));
	pAABB2->SetPosition(pAABB2->GetPosition() + v2CollisionNormal * (fCollisionDepth / 2.f));

	// apply forces to rigidbodies
	pAABB1->ApplyForce((-v2CollisionNormal * fImpulse * (pAABB1->GetMass() / fTotalMass)));
	pAABB2->ApplyForce((v2CollisionNormal * fImpulse * (pAABB2->GetMass() / fTotalMass)));

	return true;
}
