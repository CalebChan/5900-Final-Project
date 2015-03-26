#pragma once



//=============================================================================
// car.h 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstrates the creation of a surface and of vertex and index buffers
// b. demonstartes how to rpesent a surface 
//
//
// Description:
//--------------
//
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================





#ifndef NUSS_CAR 
#define NUSS_CAR 1


#include "gameobject.h"
#include "Shader.h"
//#include "camera.h"

#define NUSS_SHADERS 1


/***************************************************************************************/

// DATA STRUCTURES SECTION



struct carVertex {
	Vector4f pos;
	Vector4f normal;	// the average normal 
	Vector2f texCoord1;
	Vector2f texCoord2;
	Vector4f col;
	short numFaces;		// number of faces shared by the vertex not used

};






class Car : public gameObject
{
public:
	Car(void);
	~Car(void);
	int render(int time, Matrix4f *worldTransformation, camera *cam);
	int render(Matrix4f *worldTransformation, camera *cam);
	int updateState(int time); 	// update the object state
	int processKeys(unsigned char key); // responds to a key stroke
	int processKeys(unsigned char *key, int numKeys); // responds to  seq. of key strokes

	int createGraphicsBuffers(Shader *shader);
	int setScale(float scaleX, float scaleY, float scaleZ);
	int loadModelOBJ(char *fileName, struct carVertex **vtxBuf,  GLuint *numVtx, GLuint **indBuf,GLuint *numInd);


public:
	float dx;	// cell dimension along x axis
	float dz;	// cell dimension along z axis
		
	int * meshDecl;

	GLuint *mIndBuf;
	GLuint mNumVtx;
	struct carVertex *mVtxBuf;  // holds the vertices geometry
	GLuint mNumInd;

	GLuint mVtxVbo;
	GLuint mIndVbo;
	GLuint mVao;
	GLuint tex;		// the texture

	virtual Vector3f getHighestVector();
	virtual Vector3f getLowestVector();
	float getRadius();
private:
	void createQuad(GLuint *ind, int bottomLeft, int bottomRight, int topLeft, int topRight);
public:
	int loadTexture(char * fileName);
};



#endif