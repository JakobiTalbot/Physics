#include "PhysicsScene.h"
#include "Rigidbody.h"
#include "Application2D.h"
#include "Collision.h"
#include "Hash.h"
#include "Input.h"
#include <list>
#include <iostream>
#include "Plane.h"

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
				Sphere* s = (Sphere*)pActors;
				if (glm::distance(v2MousePos, s->GetPosition()) <= s->GetRadius())
				{
					m_pSelectedObject = pActors;
					break;
				}
			}
		}
	}
	else
	{
		Rigidbody* rb = (Rigidbody*)m_pSelectedObject;
		rb->ApplyForce((v2MousePos - rb->GetPosition()) * 0.1f);

		if (aie::Input::getInstance()->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			m_pSelectedObject = nullptr;
	}

	// fixed update
	while (fAccumulatedTime >= m_fTimeStep)
	{
		for (auto pActor : m_pActors)
		{
			pActor->FixedUpdate(m_v2Gravity, m_fTimeStep);
		}

		fAccumulatedTime -= m_fTimeStep;

		// check for collisions
		for (auto pActor : m_pActors)
		{
			Rigidbody* pRigid = (Rigidbody*)pActor;

			CheckForCollision();

			float fAspectRatio = (float)Application2D::GetInstance()->getWindowHeight() / (float)Application2D::GetInstance()->getWindowWidth();

			// check collision against boundaries
			//if (pRigid->GetPosition().x > 100.f)
			//{
			//	pRigid->ApplyForce(glm::vec2(-pRigid->GetVelocity().x * 2.f, 0.f));
			//	pRigid->SetPosition(glm::vec2(99.9f, pRigid->GetPosition().y));
			//}
			//else if (pRigid->GetPosition().x < -100.f)
			//{
			//	pRigid->ApplyForce(glm::vec2(-pRigid->GetVelocity().x * 2.f, 0.f));
			//	pRigid->SetPosition(glm::vec2(-99.9f, pRigid->GetPosition().y));
			//}

			//if (pRigid->GetPosition().y > 100.f * fAspectRatio)
			//{
			//	pRigid->ApplyForce(glm::vec2(0.f, -pRigid->GetVelocity().y * 2.f));
			//	pRigid->SetPosition(glm::vec2(pRigid->GetPosition().x, 99.9f * fAspectRatio));
			//}
			//else if (pRigid->GetPosition().y < -100.f * fAspectRatio)
			//{
			//	pRigid->ApplyForce(glm::vec2(0.f, -pRigid->GetVelocity().y * 2.f));
			//	pRigid->SetPosition(glm::vec2(pRigid->GetPosition().x, -99.9f * fAspectRatio));
			//}
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
		pSphere->ApplyForce(-(1 + 1) * glm::dot(pSphere->GetVelocity(), v2CollisionNormal) * v2CollisionNormal);
		return true;
	}
	return false;
}

bool PhysicsScene::Plane2AABB(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	return false;
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	return Plane2Sphere(pObject2, pObject1);
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	Sphere* pSphere1 = (Sphere*)pObject1;
	Sphere* pSphere2 = (Sphere*)pObject2;
	// check if distance is lower than sum of radii
	if (glm::distance(pSphere1->GetPosition(), pSphere2->GetPosition()) < (pSphere1->GetRadius() + pSphere2->GetRadius()))
	{
		pSphere1->ApplyForceToActor(pSphere2, pSphere1->GetVelocity());
		return true;
	}
	return false;
}

bool PhysicsScene::Sphere2AABB(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	return false;
}

bool PhysicsScene::AABB2Plane(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	return false;
}

bool PhysicsScene::AABB2Sphere(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	return false;
}

bool PhysicsScene::AABB2AABB(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	return false;
}
