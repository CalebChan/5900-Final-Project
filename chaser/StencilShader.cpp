#include "StencilShader.h"

int StencilShader::createShaderProgram(char *vsName, char *fsName, char *gsName, GLuint *programId)
{

	int rc = 0;

	rc = createShaderObj(vsName, GL_VERTEX_SHADER, &vertShaderid);
	if (rc == 0) {
		rc = createShaderObj(fsName, GL_FRAGMENT_SHADER, &fragShaderid);
	}
	if (rc == 0) {
		rc = createShaderObj(gsName, GL_GEOMETRY_SHADER, &geoShaderid);
	}
	if (rc == 0) {
		rc = ceateShaderProgram(vertShaderid, fragShaderid, geoShaderid, programId);
	}
	this->shaderProgramid = *programId;
	return(rc);
}

void StencilShader::setLightPosition(Vector3f pos){
	int loc = glGetUniformLocation(this->getProgId(), "gLightPos");
	if (loc == -1){
		assert(loc != -1);
	}
	glUniform3f(loc, pos.x, pos.y, pos.z);
}

void StencilShader::setMatrix(Matrix4f mat, char* name){
	Shader::copyMatrixToShader(mat, name);
}

GLint StencilShader::ceateShaderProgram(GLint vertShaderid, GLint fragShaderid, GLint geoShaderid, GLuint *shaderProgId)
{

	int rc = 0;
	// get a handle to the shader program
	shaderProgramid = glCreateProgram();
	// connect the shaders subprogram to the "main" program
	glAttachShader(shaderProgramid, vertShaderid);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr, "error in attach shaders \n");
		if (rc == GL_INVALID_VALUE) printf("error is GL_INVALID_VALUE \n");
		rc = -1;
		goto err;
	}

	glAttachShader(shaderProgramid, fragShaderid);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr, "error in attach shaders \n");
		rc = -1;
		goto err;
	}

	glAttachShader(shaderProgramid, geoShaderid);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr, "error in attach shaders \n");
		rc = -1;
		goto err;
	}

	// bind a varialbe before linking 
	//glBindAttribLocation(shaderProgramid, 7, "testNormal");
	glLinkProgram(shaderProgramid);

	// check for errors
	glGetProgramiv(shaderProgramid, GL_LINK_STATUS, &rc);

	if (rc != GL_TRUE) {
		rc = -1;
		GLsizei length;
		GLsizei bufSize = 0;
		char *error = NULL;
		fprintf(stderr, "Error when creating a shader program \n");
		//fputs("Error when creating a shader \n",stderr);
		glGetProgramiv(shaderProgramid, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char *)malloc(bufSize*sizeof(char) + 1);
		if (error != NULL) {
			glGetProgramInfoLog(shaderProgramid, bufSize, &length, error);
			printf("%s \n", error);
		}
		if (error != NULL) free(error);
	}
	else rc = 0;

	shaderStatus();  // may want to remove it.

	if (shaderProgId != NULL) *shaderProgId = shaderProgramid;
	glDetachShader(shaderProgramid, vertShaderid);
	glDetachShader(shaderProgramid, fragShaderid);
	glDetachShader(shaderProgramid, geoShaderid);
	//glDeleteShader(vertShaderid);
	//glDeleteShader(fragShaderid);
err:
	return (rc);
}