#pragma once

#include "camera.h"

class PointOfView : public camera{
public :
	Matrix4f getProjectionMatrix(Matrix4f * projMatrix);
};