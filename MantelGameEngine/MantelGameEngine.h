#pragma once

#include <chrono>
#include <list>

#include "types.h"
#include "Camera.h"
#include "GameObject.h"

enum EngineState
{
	EDITOR = 0,
	PLAY,
	PAUSE,
	STOP

};

class MyGameEngine
{
public:
	Camera* actualCamera;

	Camera camera;

	EngineState engineState = EngineState:: EDITOR;

	MyGameEngine();
	void step(std::chrono::duration<double> dt);
	void Start();
	void render();

	//add and delete game object
	void AddGameObject(GameObject* go, bool isChildren = false, GameObject* parentGo = nullptr);
	void DeleteGameObject(GameObject* go);

	int selectedGameObject = 0;

	std::list<GameObject*> hierarchy;
	std::list<GameObject*> allGameObjects;
};

