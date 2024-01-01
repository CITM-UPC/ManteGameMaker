#include "TransformComponent.h"

#include "GameObject.h"

TransformComponent::TransformComponent(GameObject* owner) : Component(owner)
{
	_transform = mat4(1.0f);
	_rotation = { 0, 0, 0 };
	_scale = { 1, 1, 1 };
	componentType = TRANSFORM;
}

TransformComponent::TransformComponent(TransformComponent* ref) : Component(ref->owner)
{
	_transform = ref->_transform;
	_rotation = ref->_rotation;
	_scale = ref->_scale;
}

TransformComponent::~TransformComponent()
{

}

mat4& TransformComponent::getTransform()
{
	return _transform;
}

mat4 TransformComponent::getGlobalTransform()
{
	if (owner->_parent != nullptr) {
		mat4 globatTransform = owner->_parent->GetComponent<TransformComponent>()->getGlobalTransform() * _transform;

		return globatTransform;
	}

	return _transform;
}

vec3& TransformComponent::getPosition()
{
	return _pos;
}

vec3& TransformComponent::getRotation()
{
	if (toleranceCheckFix(_transform[2][0], 1) == 1)
	{
		_yaw = glm::degrees(glm::half_pi<float>());
		_pitch = glm::degrees(atan2(_transform[1][2], _transform[1][1]));
		_roll = 0;

	}
	else if (toleranceCheckFix(_transform[2][0], -1) == -1)
	{
		_yaw = glm::degrees(3 * glm::half_pi<float>());
		_pitch = glm::degrees(atan2(_transform[1][2], _transform[1][1]));
		_roll = 0;
	}
	else
	{
		_yaw = glm::degrees(asin(-_transform[2][0]));
		_pitch = glm::degrees(atan2(_transform[2][1], _transform[2][2]));
		_roll = glm::degrees(atan2(_transform[1][0], _transform[0][0]));
	}

	_yaw = -_yaw;
	_pitch = -_pitch;
	_roll = -_roll;

	return _rotation;
}

vec3& TransformComponent::getScale()
{
	_scale.x = glm::length(_right);
	_scale.y = glm::length(_up);
	_scale.z = glm::length(_forward);

	return _scale;
}

vec3 TransformComponent::getRight()
{
	return _right;
}

vec3 TransformComponent::getUp()
{
	return _up;
}

vec3 TransformComponent::getForward()
{
	return _forward;
}

vec3& TransformComponent::getEulerAngles()
{
	return _rotation;
}

vec3& TransformComponent::getScaleVector()
{
	return _scale;
}

void TransformComponent::setTransformFromVectorEditing()
{
	double radPitch = glm::radians(_pitch);
	double radYaw = glm::radians(_yaw);
	double radRoll = glm::radians(_roll);

	/*if (toleranceCheckFix(sin(radPitch), 1) == 1) {
		_right = { 0, 0, -1 };
		_up = { cos(radYaw) * sin(radPitch) * sin(radRoll) - cos(radRoll) * sin(radYaw), sin(radYaw) * sin(radPitch) * sin(radRoll) + cos(radRoll) * cos(radYaw), 0 };
		_forward = { cos(radRoll) * cos(radYaw) * sin(radPitch) + sin(radYaw) * sin(radRoll), sin(radPitch) * sin(radYaw) * cos(radRoll) - cos(radYaw) * sin(radRoll), 0 };
	}
	else if (toleranceCheckFix(sin(radPitch), -1) == -1) {
		_right = { 0, 0, 1 };
		_up = { cos(radYaw) * sin(radPitch) * sin(radRoll) - cos(radRoll) * sin(radYaw), sin(radYaw) * sin(radPitch) * sin(radRoll) + cos(radRoll) * cos(radYaw), 0 };
		_forward = { cos(radRoll) * cos(radYaw) * sin(radPitch) + sin(radYaw) * sin(radRoll), sin(radPitch) * sin(radYaw) * cos(radRoll) - cos(radYaw) * sin(radRoll), 0 };
	}
	else {
		_right = { cos(radPitch) * cos(radYaw), cos(radPitch) * sin(radYaw), -sin(radPitch) };
		_up = { cos(radYaw) * sin(radPitch) * sin(radRoll) - cos(radRoll) * sin(radYaw), sin(radYaw) * sin(radPitch) * sin(radRoll) + cos(radRoll) * cos(radYaw), cos(radPitch) * sin(radRoll) };
		_forward = { cos(radRoll) * cos(radYaw) * sin(radPitch) + sin(radYaw) * sin(radRoll), sin(radPitch) * sin(radYaw) * cos(radRoll) - cos(radYaw) * sin(radRoll), cos(radPitch) * cos(radRoll) };
	}*/

	_right = { 1, 0, 0 };
	_up = { 0, 1, 0 };
	_forward = { 0, 0, 1 };

	_transform = glm::rotate(_transform, glm::radians(_pitch), vec3{ 1, 0, 0 });
	_transform = glm::rotate(_transform, glm::radians(_yaw), vec3{ 0, 1, 0 });
	_transform = glm::rotate(_transform, glm::radians(_roll), vec3{ 0, 0, 1 });

	scale(_scale);

}

void TransformComponent::translate(vec3 translation, ReferenceAxis ref)
{
	switch (ref)
	{
	case LOCAL:
		_transform = glm::translate(_transform, translation);
		break;
	case GLOBAL:
		_pos += translation;
		break;
	default:
		break;
	}
}

void TransformComponent::rotate(double degrees, const vec3& axis, ReferenceAxis ref)
{
	glm::normalize(axis);
	
	switch (ref)
	{
	case LOCAL:
		_transform = glm::rotate(_transform, glm::radians(degrees), axis);
		break;
	case GLOBAL:
		_transform = glm::rotate(_transform, glm::radians(degrees), axis * (glm::dmat3)_transform);
		break;
	default:
		break;
	}

	getRotation();
}

void TransformComponent::scale(vec3 scale)
{
	_transform = glm::scale(_transform, scale);

	getScale();
}

double TransformComponent::toleranceCheckFix(double n, int c)
{
	if (abs(n - c) < 1e-5) {
		n = c;
	}

	return n;
}
