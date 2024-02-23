#pragma once

////////////////////////////////
// Include
#include <ctime>
#include "renderSystem.h"
#include"gameObject.h"


///////////////////////////////
// Constants
const int gameObjectCountMax = 512;

//////////////////////////////////
// Class Game
class Game
{
public:
	Game();

	void setupSystem();
	void initialize();
	bool frame();
	void shutdown();

private:
	void render();
	void update( float dt );

	GameObject* createObject(GameObjectType type, float x, float y, char symbol, ConsoleColor color, ConsoleColor bg_Color);
	void destroyObject(GameObject* object);

private:
	bool m_isGameActive;
	clock_t m_clockLastFrame;

	RenderSystem m_renderSystem;
	GameObject* m_objects[gameObjectCountMax];

	float m_shipFireCooldownTime;
	float m_alientAmplitudeTime;
};