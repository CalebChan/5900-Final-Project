#include "StencilShader.h"

int StencilShader::createShaderProgram(char *vsName, char *fsName, char *gsName, GLuint *programId)
{

	int rc = 0;

	rc = createShaderObj(vsName, GL_VERTEX_SHADER, &vertShaderid);
	if (rc == 0) {
		rc = createShaderObj(fsName, GL_FRAGMENT_SHADER, &fragShaderid);
	}
	if (rc == 0) {
		rc = createShaderObj(gsName, GL_GEOMETRY_SHADER, &fragShaderid);
	}
	if (rc == 0) {
		rc = ceateShaderProgram(vertShaderid, fragShaderid, programId);
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

void StencilShader::setMatrix(Matrix4f mat){
	int loc = glGetUniformLocation(this->getProgId(), "gLightPos");
	if (loc == -1){
		assert(loc != -1);
	}
	glUniformMatrix4fv(loc, 1, true, (float *)mat.vm);
}