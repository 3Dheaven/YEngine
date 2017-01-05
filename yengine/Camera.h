#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <string>
#define PI 3.14159265359
using namespace std;

enum eCamMvmt
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

struct Plane {
	glm::vec3 point;
	glm::vec3 normal;
};

class camera
{
public:
	glm::vec3 mPosition;
	glm::mat4 projMatrix;
private:

	// Camera Attributes
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	// Eular Angles
	GLfloat mYaw;
	GLfloat mPitch;

	// Camera options
	GLfloat mMovementSpeed;
	GLfloat mMouseSensitivity;
	GLfloat mZoom;

	glm::mat3 modelMatrix;

	Plane nearPlane;
	Plane farPlane;
	Plane leftPlane;
	Plane rightPlane;
	Plane topPlane;
	Plane bottomPlane;

	float FoV = 0.785398f;
	float cosFoV;
	float ffar = 1000.0f;
	float nnear = 1.0f;

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		front.y = sin(glm::radians(mPitch));
		front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		mFront = glm::normalize(front);

		// Also re-calculate the Right and Up vector
		mRight = glm::normalize(glm::cross(mFront, mWorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		mUp = glm::normalize(glm::cross(mRight, mFront));
	}

public:

	camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up);
	~camera();

	GLfloat getZoom() const
	{
		return mZoom;
	}

	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(	mPosition,				// Camera in World Space
							mPosition + mFront,		// Looks here : at the same position, plus "direction"
							mUp						// Head is up (set to 0,-1,0 to look upside-down)
							);
	}

	void processKeyboard(eCamMvmt dirMvmt, GLfloat deltaTime)
	{
		GLfloat velocity = mMovementSpeed * deltaTime;

		if (dirMvmt == FORWARD)
			mPosition += velocity * mFront;

		if(dirMvmt == BACKWARD)
			mPosition -= velocity * mFront;

		if(dirMvmt == LEFT)
			mPosition -= mRight * velocity;

		if (dirMvmt == RIGHT)
			mPosition += mRight * velocity;
	}

	void processMouseMovement(GLfloat xoffset, GLfloat yoffset)
	{
		
		xoffset *= mMouseSensitivity;
		yoffset *= mMouseSensitivity;

		mYaw += xoffset;
		mPitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (mPitch > 89.0f)
			mPitch = 89.0f;
		if (mPitch < -89.0f)
			mPitch = -89.0f;

		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseWheel(GLfloat yoffset)
	{
		if (mZoom >= 1.0f && mZoom <= 45.0f)
			mZoom -= yoffset*0.1;
		if (mZoom <= 1.0f)
			mZoom = 1.0f;
		if (mZoom >= 45.0f)
			mZoom = 45.0f;
	}


	bool AABBInsidePlane(Plane plane, glm::vec3 max_v, glm::vec3 min_v) {
		plane.normal = modelMatrix*plane.normal;

		glm::vec3 p = min_v;

		if (plane.normal.x >= 0)
			p.x = max_v.x;
		if (plane.normal.y >= 0)
			p.y = max_v.y;
		if (plane.normal.z >= 0)
			p.z = max_v.z;

		float dotProd = glm::dot(p - plane.point, plane.normal);
		if (dotProd < 0)
			return false;
		else
			return true;
	}

	//http://www2.in.tu-clausthal.de/~zach/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
	bool AABBInsideFrustum(glm::vec3 max_v, glm::vec3 min_v) {
		modelMatrix = glm::mat3(mRight, mUp, -mFront);

		nearPlane.point = mPosition - mFront*nnear;
		if (!AABBInsidePlane(nearPlane, max_v, min_v))
			return false;

		farPlane.point = mPosition - mFront*ffar;
		if (!AABBInsidePlane(farPlane, max_v, min_v))
			return false;

		leftPlane.point = mPosition;
		if (!AABBInsidePlane(leftPlane, max_v, min_v))
			return false;

		rightPlane.point = mPosition;
		if (!AABBInsidePlane(rightPlane, max_v, min_v))
			return false;

		topPlane.point = mPosition;
		if (!AABBInsidePlane(topPlane, max_v, min_v))
			return false;

		bottomPlane.point = mPosition;
		if (!AABBInsidePlane(bottomPlane, max_v, min_v))
			return false;

		return true;
	}
};