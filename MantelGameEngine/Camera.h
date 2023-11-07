#pragma once

#include <glm/vec3.hpp>
#include "Transform.h"

struct Camera
{
public:
	Camera();
	void UpdateLookAt();

public:
	double fov;
	double aspect;
	double zNear;
	double zFar;

	Transform transform;
	vec3 lookAtPos;
	double camOffset;
};