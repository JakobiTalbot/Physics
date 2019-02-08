#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include <vector>

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	static Application2D* GetInstance() { return m_app; }

protected:

	PhysicsScene* m_pPhysicsScene;
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;

	std::vector<Sphere*> m_pSpheres;

	float m_cameraX, m_cameraY;
	float m_timer;
	
	static Application2D* m_app;
};