#pragma once

#include "camera.h"

class PointOfView : public camera{
public :
	Matrix4f getProjectionMatrix(Matrix4f * projMatrix);
	void updatePOVCamera();
	void setNextLocation(Vector3f nextLocation);
private:
	bool isSet = false;
	Vector3f nextLocation;
};