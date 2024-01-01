#pragma once

#include "BoundingBox.h"

class Graphic {
public:
	AABBox aabb;
	virtual void draw() = 0;
	virtual ~Graphic() = default;
};