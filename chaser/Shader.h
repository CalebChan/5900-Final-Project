#pragma once




#include <windows.h>


#include "glew.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include "glut.h"
#endif

#include "gl/gl.h"

#include "nuss_matrix.h"

class Shader
{
public:
	Shader(void);
	~Shader(void);
	char * readCode(char * fileName);
	int createShaderObj(char* fileName , int shaderType, GLuint *shaderid);	
	
	// functions creates a shader program.  The two shader programs (vertex and fragment) were already compiled.
	GLint ceateShaderProgram(GLint vertShaderid, GLint fragShaderid, GLuint *shaderProgId);

protected:
	GLuint vertShaderid;
	GLuint fragShaderid;
public:
	GLuint shaderProgramid;
	// creates a shader program from files vsFileName and fsFileName
	int createShaderProgram(char * vsFileName, char * fsFileName, GLuint *shaderProgramid);
	int shaderStatus(void);
	GLuint getProgId();
	int copyMatrixToShader(Matrix4f mat, char *matName);
};

