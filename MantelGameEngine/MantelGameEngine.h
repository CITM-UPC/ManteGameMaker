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
	void render();

	int selectedGameObject = 0;

	std::list<GameObject*> hierarchy;
};

