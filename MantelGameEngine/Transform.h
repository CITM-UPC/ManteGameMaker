#pragma once
#include "EGlobals.h"
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
	enum class moveType
	{
		LOCAL,
		GLOBAL,
	};

	Transform();
	~Transform();

	//Move object to pisition
	void MoveTo(vec3 position, moveType referenceFrame = moveType::GLOBAL);

	//Moves object using displacements
	void Move(vec3 displacement, moveType referenceFrame = moveType::LOCAL);

	//Rotates the object from axis
	void RotateTo(vec3f axis);

	//Rotates the object
	void Rotate(vec3f axis, moveType referenceFrame = moveType::LOCAL);

public:
	vec3 position;
	vec3 rotation;
	vec3 scale;

	vec3 forward;
	vec3 right;
	vec3 up;

	glm::mat3x3 reference_frame_mat;

};

