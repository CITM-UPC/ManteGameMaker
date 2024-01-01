#pragma once
#include "Component.h"
#include "Types.h"
#include <glm/ext/matrix_transform.hpp>

enum ReferenceAxis {
	LOCAL,
	GLOBAL
};

class TransformComponent : public Component {
public:

	TransformComponent(GameObject* owner);
	TransformComponent(TransformComponent* ref);

	~TransformComponent();

	mat4& getTransform();
	mat4 getGlobalTransform();

	vec3& getPosition();
	vec3& getRotation();
	vec3& getScale();

	vec3 getRight();
	vec3 getUp();
	vec3 getForward();

	vec3& getEulerAngles();
	vec3& getScaleVector();
	void setTransformFromVectorEditing();

	void translate(vec3 translation, ReferenceAxis ref = LOCAL);
	void rotate(double degrees, const vec3& axis, ReferenceAxis ref = LOCAL);
	void scale(vec3 scale);

private:
	double toleranceCheckFix(double n, int c = 0);

protected:

private:
	union {
		mat4 _transform;
		struct {
			vec3 _right;	double _paddingX;
			vec3 _up;		double _paddingY;
			vec3 _forward;	double _paddingZ;
			vec3 _pos;		double _paddingW;
		};
	};

	union {
		vec3 _rotation;
		struct {
			double _pitch;
			double _yaw;
			double _roll;
		};
	};

	vec3 _scale;
};