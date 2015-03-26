//=============================================================================
// camera.h 
//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstarte the usage of a camera
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a camera 
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================



//  ADD FUNCTIONS TO CHANGE THE PERSPECTIVE VIEW PARAMETERS ONE AT A TIME


#ifndef NUSS_CAMERA_FILE
#define NUSS_CAMERA_FILE 1



#include "nuss_matrix.h"

class camera
{
public:
	camera(void);
	~camera(void);
	int roll(float angleDeg);
	int pitch(float angleDeg);
	int yaw(float angleDeg);
	Vector3f getPosition(void);
	Vector3f getLookAtPoint(void);
	Vector3f getUpVector(void);
	float getSpeed(void);
	Vector3f getLookAtVector(void);



	int changePositionDelta(float dx, float dy, float dz);	// change position by relative amount
	int changePositionDelta(Vector3f *dv);				// change position by relative amount
	int changeAbsPoition(float x, float y, float z);		// change to a new position in space
	int changeAbsPosition(Vector3f *v);					// change to a new position in space
	int changeAbsPosition(Vector3f v);					// change to a new position in space
	Vector3f moveForward(float numUnits);  // moves the camera forward by the numUnits units along the lookAtVector
	void setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector);
	void setCamera(Vector4f position, Vector4f lookAtPoint, Vector4f upVector);
	int updateSpeed(float speed);				// change the speed of the camera motion
	int changeLookAtVector(float x, float y, float z);  	
	Matrix4f getViewMatrix(Matrix4f * viewMatrix);
	Matrix4f getProjectionMatrix(Matrix4f * projMatrix);
	int setPerspectiveView(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
	int zoomIn(void);
	int zoomIn(float zoom);
	int zoomOut(void);
	int zoomOut(float zoom);


private:
	int updateOrientation(Vector3f rotVector, float angleRad); // update the camera's orientation in space
	
	// left as public for debugging purposes
public:
	Matrix4f rotMat;
	Vector4f position_old;
	Vector4f upVector_old;
	Vector4f lookAtVector_old;
	Vector3f position;
	Vector3f upVector;
	Vector3f lookAtVector;
	float speed;

	// perspective projection fields
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;
};

#endif