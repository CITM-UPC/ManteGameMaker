#pragma once

#include <chrono>

#include "types.h"
#include "Camera.h"

class MantelGameEngine
{
public:

	Camera camera;

	MantelGameEngine();
	void step(std::chrono::duration<double> dt);
	void render();
};

