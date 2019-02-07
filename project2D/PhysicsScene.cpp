#include "PhysicsScene.h"
#include "Rigidbody.h"
#include <list>
#include <iostream>

PhysicsScene::PhysicsScene()
{
	// initialise variables
	m_fTimeStep = 0.01f;
	m_v2Gravity = glm::vec2(0, 0);
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
	static std::list<PhysicsObject*> dirty;

	// update physics at a fixed time step
	static float fAccumulatedTime = 0.f;
	fAccumulatedTime += fDeltaTime;

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
			for (auto pOther : m_pActors)
			{
				// ignore if both actors are the same actor
				if (pActor == pOther)
					continue;

				// ignore if both are already in list
				if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&
					std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
					continue;

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
				if (pRigid->CheckCollision(pOther))
				{
					pRigid->ApplyForceToActor(dynamic_cast<Rigidbody*>(pOther), pRigid->GetVelocity() * pRigid->GetMass());
					dirty.push_back(pRigid);
					dirty.push_back(pOther);
				}
			}
		}
		dirty.clear();
	}
}

void PhysicsScene::UpdateGizmos()
{
	for (auto pActor : m_pActors)
	{
		pActor->MakeGizmo();
	}
}