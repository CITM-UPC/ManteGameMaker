#include "Camera.h"

Camera::Camera() : fov(60), aspect(4.0 / 3.0), zNear(0.1), zFar(100), camOffset(3)
{
	transform.position = vec3(5, 2, -5);
	UpdateLookAt();
}

void Camera::UpdateLookAt()
{
	lookAtPos = (transform.forward * camOffset);
	lookAtPos += transform.position;
}