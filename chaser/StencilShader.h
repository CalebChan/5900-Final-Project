#pragma once

#include "Shader.h"

class StencilShader : public Shader{
protected:
	GLuint geoShaderid;

public:

	int createShaderProgram(char *vsName, char *fsName, char *gsName, GLuint *programId);

	void setLightPosition(Vector3f pos);

	void setMatrix(Matrix4f mat);
};