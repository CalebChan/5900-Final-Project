#pragma once

#include "glew.h"
#include "glut.h"
#include "gl/gl.h"

#include <random>
#include <vector>
#include "Shader.h"


using namespace std;

class VisibleTexture {
public:
	VisibleTexture();
	~VisibleTexture();

	void bindTexture();
	void unbindTexture();

	bool checkIfValidPointOnTexture(Vector3f local, Matrix4f mvpMat);
	Vector3f generateValidDirections(Vector3f loc, Vector3f dir, Matrix4f mvpMat, float searchAngle, float madDist, int sampleSize);
private:

	GLuint buffer;
	GLuint texture;

	uniform_real_distribution<double> engine;
	default_random_engine generator;

	void createTexture();

	
};