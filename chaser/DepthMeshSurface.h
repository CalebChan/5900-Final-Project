#pragma once

#include "meshSurface.h"
#include "camera.h"

class DepthMeshSurface : public meshSurface{
private:
	GLuint otherTex;
	camera *overheadCam;
public:
	DepthMeshSurface();
	void setMeshTexture(GLuint texture);
	void revertTexture();

	void setOverheadCamera(camera *cam);
	int render(Matrix4f *worldMat, camera *cam);
};