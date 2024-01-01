#pragma once

#include <array>
#include "Types.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "BoundingBox.h"


struct Plane
{
	// unit vector
	vec3 normal;

	// position
	vec3 pos;

	bool IsPointOnPositiveSide(vec3 point);
};

struct Frustum
{
	union {
		std::array<Plane, 6> faces;
		struct {
			Plane topFace;
			Plane bottomFace;

			Plane rightFace;
			Plane leftFace;

			Plane farFace;
			Plane nearFace;
		};
	};
	
	bool IsBoundingBoxInFrustum(AABBox aabb);
};

struct Camera
{
	double fov;
	double aspect;
	double zNear;
	double zFar;

	// local frustum, will have to apply transformation before using it
	Frustum createFrustum();

	Frustum createFrustum(mat4 transform);

	// local frustum, will be drawn after applying the transform matrix in the render
	void drawFrustum();

	Camera() : fov(60), aspect(16.0 / 9.0), zNear(0.1), zFar(100) {}
};
