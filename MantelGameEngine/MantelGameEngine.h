#pragma once

#include <chrono>

#include "types.h"
#include "Camera.h"

class MantelGameEngine
{
public:

	enum class RenderModes {RELEASE, DEBUG};

	Camera camera;

	void step(std::chrono::duration<double> dt);
	void render(RenderModes renderMode);
};

