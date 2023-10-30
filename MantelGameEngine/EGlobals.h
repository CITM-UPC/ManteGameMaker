#pragma once
#include <windows.h>
#include <stdio.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#define ELOG(format, ...) engineLog(__FILE__, __LINE__, format, __VA_ARGS__);

void engineLog(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

using vec2 = glm::dvec2;
using vec3 = glm::dvec3;
using vec4 = glm::dvec4;
using mat4 = glm::dmat4;

using vec2f = glm::vec2;
using vec3f = glm::vec3;
using vec4f = glm::vec4;

enum engine_status
{
	ENGINE_UPDATE_CONTINUE = 1,
	ENGINE_UPDATE_STOP,
	ENGINE_UPDATE_ERROR
};