#pragma once

#include "glew.h"
#include "glut.h"
#include "gl/gl.h"

class VisibleTexture {
public:
	VisibleTexture();
	~VisibleTexture();

	void bindTexture();
	void unbindTexture();
private:

	GLuint buffer;
	GLuint texture;

	void createTexture();
};