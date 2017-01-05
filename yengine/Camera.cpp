#include "camera.h"

camera::camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up):
mPosition(position), 
mFront(direction), 
mUp(up)
{
	mYaw = -90.0f;
	mPitch = 0.0f;
	mMovementSpeed = 100.0f;
	mMouseSensitivity = 0.25f;
	mZoom = 45.0f;
	mWorldUp = mUp;
	updateCameraVectors();



	farPlane;
	farPlane.point = position + direction*ffar;
	farPlane.normal = -direction;

	leftPlane;
	leftPlane.point = position;
	float angle = PI / 2.0f + FoV / 2.0f + 0.1f;
	leftPlane.normal = glm::vec3(glm::rotate(glm::mat4(), angle, up)*glm::vec4(direction, 1.0f));

	rightPlane;
	rightPlane.point = position;
	rightPlane.normal = glm::vec3(glm::rotate(glm::mat4(), -angle, up)*glm::vec4(direction, 1.0f));

	topPlane;
	topPlane.point = position;
	topPlane.normal = glm::vec3(glm::rotate(glm::mat4(), angle, mRight)*glm::vec4(direction, 1.0f));

	bottomPlane;
	bottomPlane.point = position;
	bottomPlane.normal = glm::vec3(glm::rotate(glm::mat4(), -angle, mRight)*glm::vec4(direction, 1.0f));


}

camera::~camera()
{
	
}
