#pragma once

//=============================================================================
// skybox.h
//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstarte the usage of a skybox
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a skybox 
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



// setting the different callback functions of OpenGL
#include "glew.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include "glut.h"
#endif

#include "gl/gl.h"

#include "camera.h"
#include "Shader.h"



class SkyBox

{
public:
	SkyBox(void);
	~SkyBox(void);
	int loadSkybox(char ** texFileName);
	void SkyBox::displaySkybox(camera cam);
	GLuint getTexHandle(void);



private:
	GLuint texHandle;  // this is the texture handle
	GLuint shaderProg;	// shader for drawing the skybox
	Shader s;			// shader class object
	GLuint vboCube;		// consider removing to a static global in the cpp file
	GLuint iboCube;		// consider removing to a static global in the cpp file
	GLuint texCube;		// consider removing to a static global in the cpp file

	int createCube(void);

public:
	int init(void);
	int loadColourTexture(void);
};

