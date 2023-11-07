#pragma once

#include <chrono>
#include <list>

#include "types.h"
#include "Camera.h"
#include "GameObject.h"


class MyGameEngine
{
public:
	Camera camera;

	MyGameEngine();
	void step(std::chrono::duration<double> dt);
	void Start();
	void render();

	//add and delete game object
	void AddGameObject(GameObject* go);
	void DeleteGameObject(GameObject* go);

	int selectedGameObject = 0;

	std::list<GameObject*> hierarchy;
};

