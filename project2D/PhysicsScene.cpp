#include "PhysicsScene.h"
#include "Rigidbody.h"
#include "Application2D.h"
#include "Collision.h"
#include "Hash.h"
#include "Input.h"
#include <list>
#include <iostream>

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

	// DRAG OBJECT
	if (!m_pSelectedObject)
	{
		if (aie::Input::getInstance()->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
		{
			for (auto pActors : m_pActors)
			{
				glm::vec2 v2MousePos = glm::vec2(aie::Input::getInstance()->getMouseX(), aie::Input::getInstance()->getMouseX());
				Sphere* s = (Sphere*)pActors;
				if (glm::distance(v2MousePos, s->GetPosition()) <= s->GetRadius())
				{
					m_pSelectedObject == pActors;
					break;
				}
			}
		}
	}
	else
	{
		if (aie::Input::getInstance()->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
			m_pSelectedObject = nullptr;

		Rigidbody* rb = (Rigidbody*)m_pSelectedObject;
		glm::vec2 v2MousePos = glm::vec2(aie::Input::getInstance()->getMouseX(), aie::Input::getInstance()->getMouseX());
		rb->ApplyForce((v2MousePos - rb->GetPosition()) * 10.f);
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

			// check collision against other actors
			for (auto pOther : m_pActors)
			{
				// ignore if both actors are the same actor
				if (pActor == pOther)
					continue;

				// create new collision
				Collision* collision = new Collision(pActor, pOther);

				// ignore if collision has already occurred
				for (auto pCollision : collisions)
				{
					char collisionAddress[sizeof(Collision)];
					sprintf(collisionAddress, "%i", collision);

					char collisionAddress1[sizeof(Collision)];
					sprintf(collisionAddress1, "%i", pCollision);

					// compare collision hashes
					Hash h;
					if (h.DoHash(collisionAddress, strlen(collisionAddress)) == h.DoHash(collisionAddress1, strlen(collisionAddress1)))
					{
						continue;
					}
				}

				if (pRigid->CheckCollision(pOther))
				{
					pRigid->ApplyForceToActor((Rigidbody*)pOther, pRigid->GetVelocity());
					collisions.push_back(collision);
				}
				else 
				{
					delete collision;
					collision = nullptr;
				}
			}

			float fAspectRatio = (float)Application2D::GetInstance()->getWindowHeight() / (float)Application2D::GetInstance()->getWindowWidth();

			// check collision against boundaries
			if (pRigid->GetPosition().x > 100.f)
			{
				pRigid->ApplyForce(glm::vec2(-pRigid->GetVelocity().x * 2.f, 0.f));
				pRigid->SetPosition(glm::vec2(99.9f, pRigid->GetPosition().y));
			}
			else if (pRigid->GetPosition().x < -100.f)
			{
				pRigid->ApplyForce(glm::vec2(-pRigid->GetVelocity().x * 2.f, 0.f));
				pRigid->SetPosition(glm::vec2(-99.9f, pRigid->GetPosition().y));
			}

			if (pRigid->GetPosition().y > 100.f * fAspectRatio)
			{
				pRigid->ApplyForce(glm::vec2(0.f, -pRigid->GetVelocity().y * 2.f));
				pRigid->SetPosition(glm::vec2(pRigid->GetPosition().x, 99.9f * fAspectRatio));
			}
			else if (pRigid->GetPosition().y < -100.f * fAspectRatio)
			{
				pRigid->ApplyForce(glm::vec2(0.f, -pRigid->GetVelocity().y * 2.f));
				pRigid->SetPosition(glm::vec2(pRigid->GetPosition().x, -99.9f * fAspectRatio));
			}
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