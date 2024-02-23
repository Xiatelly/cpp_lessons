///////////////////////////
// Include
#include "game.h"
#include <cstdlib>
#include<cstdio>
#include<cmath>
#include"level.h"
#include"input.h"

////////////////////////
// Class Game
Game::Game()
{
	m_isGameActive = true;
	m_clockLastFrame = 0;

	for (int i = 0; i < gameObjectCountMax; i++)
		m_objects[i] = 0;

	m_shipFireCooldownTime = 0.0;
	m_alientAmplitudeTime = 0.0;
}

void Game::setupSystem()
{
	srand( time( 0 ) );
	m_renderSystem.initialize();
}

void Game::initialize()
{

}

bool Game::frame()
{
	// Calculate delta time
	clock_t clockNow = clock();
	clock_t deltaClock = clockNow - m_clockLastFrame;
	float deltaTime = float(deltaClock) / CLOCKS_PER_SEC;
	m_clockLastFrame = clockNow;

	render();
	update(deltaTime);

	return m_isGameActive;
}

void Game::shutdown()
{
	for (int i = 0; i < gameObjectCountMax; i++)
		if (m_objects[i] != 0)
		{
			delete m_objects[i];
			m_objects[i] = 0;
		}
}

void Game::render()
{
	// Start frame
	m_renderSystem.clear();

	//Draw all gameobjects
	int objectCount = 0;
	for(int i = 0; i < gameObjectCountMax; i++)
		if (m_objects[i] != 0)
		{
			m_objects[i]->render(&m_renderSystem);
			objectCount++;
		}

	// Draw objects count
	//
	char buff[64];
	sprintf_s(buff, "Objs: %d", objectCount);
	m_renderSystem.drawText(0, 0, buff, ConsoleColor_Gray, ConsoleColor_Black);
	
	 
	// End frame
	m_renderSystem.flush();
}

GameObject* Game::createObject(GameObjectType type, float x, float y, char symbol, ConsoleColor color, ConsoleColor bgColor)
{
	// Find free pointer and create object
	for (int i = 0; i < gameObjectCountMax; i++)
	{
		if (m_objects[i] == 0)
		{
			GameObject* object = new GameObject();

			object->setType(type);
			object->setX(x);
			object->setY(y);
			object->setSymbol(symbol);
			object->setColor(color);
			object->setBackgroundColor(bgColor);

			m_objects[i] = object;
			return object;
		}
	}

	return 0;
}

void Game::destroyObject(GameObject* object)
{
	for (int i = 0; i < gameObjectCountMax; i++)
	{
		if (m_objects[i] == object)
		{
			delete m_objects[i];
			m_objects[i] = 0;
			return;
		} 
	}
}

void Game::update(float dt)
{

}