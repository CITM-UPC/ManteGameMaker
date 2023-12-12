#pragma once

#include <glm/vec3.hpp>
#include "Transform.h"
#include "GameObject.h"

class GameObject;

struct Camera
{
public:
	Camera();
	void UpdateLookAt();

	void LookAtGameObject(GameObject* go);

public:
	double fov;
	double aspect;
	double zNear;
	double zFar;

	Transform transform;
	vec3 lookAtPos;
	double camOffset;
};