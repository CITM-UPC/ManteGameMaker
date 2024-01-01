#include "CameraComponent.h"

#include "GameObject.h"

CameraComponent::CameraComponent(GameObject* owner) : Component(owner)
{
	componentType = CAMERA;
	transform = owner->GetComponent<TransformComponent>();
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::setCamera(Camera camera)
{
	_camera = camera;
}

Camera* CameraComponent::getCamera()
{
	return &_camera;
}

TransformComponent* CameraComponent::getTransform()
{
	return transform;
}
