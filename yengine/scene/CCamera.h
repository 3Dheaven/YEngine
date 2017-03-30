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

struct Plane 
{
	glm::vec3 point;
	glm::vec3 normal;
};

class CCamera
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

		// Camera options
		GLfloat mMovementSpeed;
		GLfloat mMouseSensitivity;
		GLfloat mZoom;
		
		// Eular Angles
		GLfloat mYaw;
		GLfloat mPitch;

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
		void updateCameraVectors();

	public:

		CCamera(glm::vec3 position, glm::vec3 direction, glm::vec3 up);
		CCamera() {};
		~CCamera();

		void update();

		void setYaw(float y);

		GLfloat getZoom() const;

		glm::mat4 getProjectionMatrix();

		glm::mat4 getViewMatrix();

		void processKeyboard(eCamMvmt dirMvmt, GLfloat deltaTime);

		void processMouseMovement(GLfloat xoffset, GLfloat yoffset);

		void processMouseWheel(GLfloat yoffset);
	
		bool AABBInsidePlane(Plane plane, glm::vec3 max_v, glm::vec3 min_v);

		//http://www2.in.tu-clausthal.de/~zach/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
		bool AABBInsideFrustum(glm::vec3 max_v, glm::vec3 min_v);
};