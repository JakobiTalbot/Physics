#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include <math.h>
#include "AABB.h"
#include "Sphere.h"
#include "Plane.h"
#include <time.h>
Application2D* Application2D::m_app = nullptr;

Application2D::Application2D() {
	m_app = this;
}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_pPhysicsScene = new PhysicsScene();
	m_pPhysicsScene->SetGravity(glm::vec2(0, -150));
	m_pPhysicsScene->SetTimeStep(1.f/60.f);
	srand(time(NULL));
	for (int i = 0; i < 2; ++i)
	{		
		Sphere* ball = new Sphere(glm::vec2((rand() % 200) - 100, (rand() % 100) - 50), glm::vec2((rand() % 60) - 30, (rand() % 60) - 30), 1.f, 3.f, 1.f, glm::vec4((rand() % 1000)/1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, 1));
		m_pPhysicsScene->AddActor(ball);
	}

	AABB* box = new AABB({ 0, -20 }, { 0, 0 }, 0.f, 1.f, { 10.f, 10.f }, 1.f, glm::vec4((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, 1));
	m_pPhysicsScene->AddActor(box);
	//AABB* box1 = new AABB({ 0, 20 }, { 0, -10 }, 0.f, 1.f, { 3.f, 3.f }, 1.f, glm::vec4((rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, (rand() % 1000) / 1000.0f, 1));
	//m_pPhysicsScene->AddActor(box1);
	setBackgroundColour(0.2f, 0.2f, 0.2f, 1.f);
	Plane* topPlane = new Plane(glm::vec2(0.f, -1.f), -55.f);
	m_pPhysicsScene->AddActor(topPlane);
	Plane* leftPlane = new Plane(glm::vec2(1.f, 0.f), -99.f);
	m_pPhysicsScene->AddActor(leftPlane);
	Plane* bottomPlane = new Plane(glm::vec2(0.f, 1.f), -55.f);
	m_pPhysicsScene->AddActor(bottomPlane);
	Plane* rightPlane = new Plane(glm::vec2(-1.f, 0.f), -99.f);
	m_pPhysicsScene->AddActor(rightPlane);

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_cameraX = 0;
	m_cameraY = 0;
	m_timer = 0;

	return true;
}

void Application2D::shutdown() {
	
	delete m_font;
	m_font = nullptr;
	delete m_2dRenderer;
	m_2dRenderer = nullptr;
	delete m_pPhysicsScene;
	m_pPhysicsScene = nullptr;

	aie::Gizmos::destroy();
}

void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	// clear all gizmos
	aie::Gizmos::clear();

	// update physics scene
	m_pPhysicsScene->Update(deltaTime);
	m_pPhysicsScene->UpdateGizmos();

	// fuck you jakobi

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw gizmos
	float fAspectRatio = (float)getWindowWidth() / (float)getWindowHeight();
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / fAspectRatio, 100 / fAspectRatio, -1.f, 1.f));

	// done drawing sprites
	m_2dRenderer->end();
}