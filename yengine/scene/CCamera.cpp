#include "CCamera.h"

CCamera::CCamera(glm::vec3 position, glm::vec3 direction, glm::vec3 up):
mPosition(position), mFront(direction), mUp(up)
{
	mYaw = -90.0f;
	mPitch = 0.0f;
	mMovementSpeed = 100.0f;
	mMouseSensitivity = 0.1f;
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

CCamera::~CCamera()
{
	
}

void 
CCamera::update()
{
	updateCameraVectors();
}

void
CCamera::setYaw(float y)
{
	mYaw = y;
}

bool 
CCamera::AABBInsideFrustum(glm::vec3 max_v, glm::vec3 min_v)
{
	modelMatrix = glm::mat3(mRight, mUp, -mFront);

	nearPlane.point = mPosition - mFront*nnear;
	if (!AABBInsidePlane(nearPlane, max_v, min_v))
	{
		return false;
	}

	farPlane.point = mPosition - mFront*ffar;
	if (!AABBInsidePlane(farPlane, max_v, min_v))
	{
		return false;
	}

	leftPlane.point = mPosition;
	if (!AABBInsidePlane(leftPlane, max_v, min_v))
	{
		return false;
	}

	rightPlane.point = mPosition;
	if (!AABBInsidePlane(rightPlane, max_v, min_v))
	{
		return false;
	}

	topPlane.point = mPosition;
	if (!AABBInsidePlane(topPlane, max_v, min_v))
	{
		return false;
	}

	bottomPlane.point = mPosition;
	if (!AABBInsidePlane(bottomPlane, max_v, min_v))
	{
		return false;
	}

	return true;
}

bool 
CCamera::AABBInsidePlane(Plane plane, glm::vec3 max_v, glm::vec3 min_v)
{
	plane.normal = modelMatrix*plane.normal;

	glm::vec3 p = min_v;

	if (plane.normal.x >= 0)
	{
		p.x = max_v.x;
	}

	if (plane.normal.y >= 0)
	{
		p.y = max_v.y;
	}

	if (plane.normal.z >= 0)
	{
		p.z = max_v.z;
	}

	float dotProd = glm::dot(p - plane.point, plane.normal);

	if (dotProd < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void 
CCamera::processMouseWheel(GLfloat yoffset)
{
	if (mZoom >= 1.0f && mZoom <= 45.0f)
	{
		mZoom -= yoffset*0.1;
	}

	if (mZoom <= 1.0f)
	{
		mZoom = 1.0f;
	}

	if (mZoom >= 45.0f)
	{
		mZoom = 45.0f;
	}
}

void 
CCamera::processMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
	xoffset *= mMouseSensitivity;
	yoffset *= mMouseSensitivity;

	mYaw += xoffset;
	mPitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (mPitch > 89.0f)
	{
		mPitch = 89.0f;
	}

	if (mPitch < -89.0f)
	{
		mPitch = -89.0f;
	}

	updateCameraVectors();
}

void 
CCamera::processKeyboard(eCamMvmt dirMvmt, GLfloat deltaTime)
{
	GLfloat velocity = mMovementSpeed * deltaTime;

	if (dirMvmt == FORWARD)
	{
		mPosition += velocity * mFront;
	}

	if (dirMvmt == BACKWARD)
	{
		mPosition -= velocity * mFront;
	}

	if (dirMvmt == LEFT)
	{
		mPosition -= mRight * velocity;
	}

	if (dirMvmt == RIGHT)
	{
		mPosition += mRight * velocity;
	}
}

glm::mat4 
CCamera::getViewMatrix()
{
	return glm::lookAt(mPosition,				// Camera in World Space
		mPosition + mFront,		// Looks here : at the same position, plus "direction"
		mUp						// Head is up (set to 0,-1,0 to look upside-down)
	);
}

GLfloat 
CCamera::getZoom() const
{
	return mZoom;
}

void  
CCamera::updateCameraVectors()
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