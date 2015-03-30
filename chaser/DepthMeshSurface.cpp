#include "DepthMeshSurface.h"
#include "gameApp.h"


DepthMeshSurface::DepthMeshSurface(){
	this->otherTex = -1;
}

void DepthMeshSurface::setMeshTexture(GLuint texture){
	this->otherTex = texture;
}

void DepthMeshSurface::revertTexture(){
	this->otherTex = tex;
}

void DepthMeshSurface::setOverheadCamera(camera *cam){
	this->overheadCam = cam;
}

int DepthMeshSurface::render(Matrix4f *worldMat, camera *cam)
{

	Matrix4f modelWorldMat;  // model and world transformation. 
	Matrix4f viewMat, projMat;

	glUseProgram(shader->getProgId());

	// set the transformation of the object
	modelWorldMat = Matrix4f::translation(this->mPosition)*Matrix4f::scale(mScaleX, mScaleY, mScaleZ);
	if (worldMat != NULL) modelWorldMat = *worldMat *  modelWorldMat;

	// set the camera position
	viewMat = cam->getViewMatrix(NULL);
	modelWorldMat = viewMat * modelWorldMat;
	// transfer to shader 
	shader->copyMatrixToShader(modelWorldMat, "modelWorldViewMat");
	// set the camera position
	projMat = cam->getProjectionMatrix(NULL);
	// transfer to shader 
	shader->copyMatrixToShader(projMat, "projMat");

	if (this->otherTex == -1){
		this->otherTex = tex;
	}
	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->otherTex);
	GLuint texLoc = glGetUniformLocation(this->shader->getProgId(), "texHandle");
	glUniform1i(texLoc, 3);
	GLint ttt = 0;
	glGetUniformiv(this->shader->getProgId(), texLoc, &ttt);

	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, mNumInd, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;
}