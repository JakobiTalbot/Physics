#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>
#include <glm\ext.hpp>
#include "Sphere.h"

Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	
	m_2dRenderer = new aie::Renderer2D();

	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_pPhysicsScene = new PhysicsScene();
	m_pPhysicsScene->SetGravity(glm::vec2(0, -10));
	m_pPhysicsScene->SetTimeStep(0.01f);

	Sphere* ball;
	ball = new Sphere(glm::vec2(-40, 0), glm::vec2(10, 30), 3.f, 1, glm::vec4(1, 0, 0, 1));
	m_pPhysicsScene->AddActor(ball);

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
}

void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_pPhysicsScene->Update(deltaTime);
	m_pPhysicsScene->UpdateGizmos();

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

	static float fAspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / fAspectRatio, 100 / fAspectRatio, -1.f, 1.f));

	// done drawing sprites
	m_2dRenderer->end();
}