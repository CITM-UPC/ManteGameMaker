#pragma once

#include "EGlobals.h"
#include "MatrixLogic.h"

struct Camera
{
public:
	Camera();
	void UpdateLookAt();

public:
	double fov;
	double aspectRatio;
	double clippingPlaneViewNear;
	double clippingPlaneViewFar;

	MatrixLogic transform;
	vec3 lookAtPos;
	double camOffset;
};

