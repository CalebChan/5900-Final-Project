#pragma once

#include "Shader.h"

class StencilShader : public Shader{
protected:
	GLuint geoShaderid;

public:

	int createShaderProgram(char *vsName, char *fsName, char *gsName, GLuint *programId);
	GLint ceateShaderProgram(GLint vertShaderid, GLint fragShaderid, GLint geoShaderid, GLuint *shaderProgId);

	void setLightPosition(Vector3f pos);

	void setMatrix(Matrix4f mat, char* name);
};