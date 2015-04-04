#include "VisibleTexture.h"

#include <assert.h>
#include "Constant.h"
#include <stdio.h>



VisibleTexture::VisibleTexture(){
	engine = uniform_real_distribution<double>(0.0, 1.0);
	createTexture();
}

void VisibleTexture::bindTexture(){
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
}

void VisibleTexture::unbindTexture(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VisibleTexture::createTexture(){
	buffer = 0;
	glGenFramebuffers(1, &buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLenum frameStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (frameStatus != GL_FRAMEBUFFER_COMPLETE){
		printf("GL_FRAMEBUFFER_COMPLETE for visible texture failed, CANNOT use FBO\n");
		assert(false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Vector3f VisibleTexture::generateValidDirections(Vector3f loc, Vector3f dir, Matrix4f mvpMat, float searchAngle, float maxDist, int sampleSize){

	float halfSearch = searchAngle / 2;

	vector<Vector3f> possibleDir;
	bindTexture();
	while (possibleDir.size() == 0){
		double angle = engine(generator) * halfSearch;
		bool right = engine(generator) > 0.5;
		double dist = engine(generator) * maxDist;

		Vector3f newLocal = Vector3f(0.0, loc.y, 0.0);

		double angleX = dist * cos(angle);
		double angleY = dist * sin(angle);

		Vector3f normDir = Vector3f(dir);
		normDir.normalize();
		
		if (right){
			newLocal.x = loc.x + angleX * normDir.x;
			newLocal.z = loc.z + angleY * normDir.z;
		}
		else{
			newLocal.x = loc.x - angleX * normDir.x;
			newLocal.z = loc.z - angleY * normDir.z;
		}

		if (checkIfValidPointOnTexture(newLocal, mvpMat)){
			possibleDir.push_back(newLocal);
		}
	}
	unbindTexture();
	return possibleDir.at(0);
}

bool VisibleTexture::checkIfValidPointOnTexture(Vector3f local, Matrix4f mvpMat){
	Vector4f texLocal = mvpMat * Vector4f(local, 1);
	texLocal = texLocal / texLocal.w;
	struct{	GLubyte red, green, blue;} pixel;
	
	glReadPixels(texLocal.x, texLocal.z, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
	
	//printf("Pixel Red : %d Blue : %d Green : %d", pixel.red, pixel.blue, pixel.green);
	//printf(" X : %f Y : %f Z : %f\n", texLocal.x, texLocal.y, texLocal.z);
	if (pixel.red >= 128){
		return true;
	}
	
	return false;
}