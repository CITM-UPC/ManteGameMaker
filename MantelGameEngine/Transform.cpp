#include "Transform.h"

Transform::Transform()
{
	position = vec3(0, 0, 0);
	rotation = vec3(0, 0, 0);
	scale = vec3(1, 1, 1);

	right = vec3(1, 0, 0);
	up = vec3(0, 1, 0);
	forward = vec3(0, 0, 1);

	reference_frame_mat = glm::mat3(1.0); //Identity
}

Transform::~Transform() {}

void Transform::MoveTo(vec3 position, moveType referenceFrame)
{
	vec3 resultingVector = position;

	if (referenceFrame == moveType::LOCAL)
	{
		resultingVector = reference_frame_mat * position;
		this->position += resultingVector;
		return;
	}

	this->position = resultingVector;
}

void Transform::Move(vec3 displacement, moveType referenceFrame)
{
	vec3 resultingVector = displacement;

	if (referenceFrame == moveType::LOCAL)
		resultingVector = reference_frame_mat * displacement;

	position += resultingVector;
}

void Transform::RotateTo(vec3f axis)
{
	//Reset the referenceFrame to world default
	right = vec3(1, 0, 0);
	up = vec3(0, 1, 0);
	forward = vec3(0, 0, 1);

	//Generate the rotation matrix that corresponds to the rotation
	glm::mat3x3 rotX = glm::mat3x3(1, 0, 0,
		0, glm::cos(glm::radians(axis.x)), -glm::sin(glm::radians(axis.x)),
		0, glm::sin(glm::radians(axis.x)), glm::cos(glm::radians(axis.x)));

	glm::mat3x3 rotY = glm::mat3x3(glm::cos(glm::radians(axis.y)), 0, glm::sin(glm::radians(axis.y)),
		0, 1, 0,
		-glm::sin(glm::radians(axis.y)), 0, glm::cos(glm::radians(axis.y)));

	glm::mat3x3 rotZ = glm::mat3x3(glm::cos(glm::radians(axis.z)), -glm::sin(glm::radians(axis.z)), 0,
		glm::sin(glm::radians(axis.z)), glm::cos(glm::radians(axis.z)), 0,
		0, 0, 1);

	glm::mat3x3 rotMatrix = rotZ * rotY * rotX;

	//Apply the generated rotation matrix to the existing director vectors
	forward = rotMatrix * forward;
	right = rotMatrix * right;
	up = rotMatrix * up;

	reference_frame_mat[0].x = right.x;		reference_frame_mat[0].y = right.y;		reference_frame_mat[0].z = right.z;
	reference_frame_mat[1].x = up.x;		reference_frame_mat[1].y = up.y;		reference_frame_mat[1].z = up.z;
	reference_frame_mat[2].x = forward.x;	reference_frame_mat[2].y = forward.y;	reference_frame_mat[2].z = forward.z;
}

void Transform::Rotate(vec3f axis, moveType referenceFrame)
{
	//Translate the axis to the local refernce frame to apply the rotation

	reference_frame_mat[0].x = right.x;		reference_frame_mat[0].y = right.y;		reference_frame_mat[0].z = right.z;
	reference_frame_mat[1].x = up.x;		reference_frame_mat[1].y = up.y;		reference_frame_mat[1].z = up.z;
	reference_frame_mat[2].x = forward.x;	reference_frame_mat[2].y = forward.y;	reference_frame_mat[2].z = forward.z;

	glm::vec3 vecInRefFrame = axis;
	if (referenceFrame == moveType::LOCAL)
		vecInRefFrame = reference_frame_mat * axis;


	//Generate the rotation matrix that corresponds to the rotation
	glm::mat3x3 rotX = glm::mat3x3(1, 0, 0,
		0, glm::cos(glm::radians(vecInRefFrame.x)), -glm::sin(glm::radians(vecInRefFrame.x)),
		0, glm::sin(glm::radians(vecInRefFrame.x)), glm::cos(glm::radians(vecInRefFrame.x)));

	glm::mat3x3 rotY = glm::mat3x3(glm::cos(glm::radians(vecInRefFrame.y)), 0, glm::sin(glm::radians(vecInRefFrame.y)),
		0, 1, 0,
		-glm::sin(glm::radians(vecInRefFrame.y)), 0, glm::cos(glm::radians(vecInRefFrame.y)));

	glm::mat3x3 rotZ = glm::mat3x3(glm::cos(glm::radians(vecInRefFrame.z)), -glm::sin(glm::radians(vecInRefFrame.z)), 0,
		glm::sin(glm::radians(vecInRefFrame.z)), glm::cos(glm::radians(vecInRefFrame.z)), 0,
		0, 0, 1);

	glm::mat3x3 rotMatrix = rotZ * rotY * rotX;

	//Apply the generated rotation matrix to the existing director vectors
	forward = rotMatrix * forward;
	right = rotMatrix * right;
	up = rotMatrix * up;
}
