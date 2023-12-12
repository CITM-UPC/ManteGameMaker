#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() : fov(60), aspect(4.0 / 3.0), zNear(0.1), zFar(100), camOffset(3)
{
	transform.position = vec3(0, 1, -10);
	UpdateLookAt();
}

void Camera::UpdateLookAt()
{
	lookAtPos = (transform.forward * camOffset);
	lookAtPos += transform.position;
}

void Camera::LookAtGameObject(GameObject* go)
{
    float distance = 5.0f;

	vec3 targetPosition = go->transform.position;

    // Calculate the direction vector from the camera to the game object
	vec3 cameraToTarget = targetPosition - transform.position;
	cameraToTarget = normalize(cameraToTarget);
	vec3 newCameraPosition = targetPosition - cameraToTarget * vec3(distance,distance,distance);

	vec3 forwardVector = normalize(targetPosition - newCameraPosition);

	vec3 upVector = vec3(0.0, 1.0, 0.0);  // Assuming up is always in the Y-axis
	vec3 rightVector = normalize(cross(upVector, forwardVector));

	// Update the camera's position and orientation
	transform.position = newCameraPosition;


	lookAtPos = targetPosition;
}