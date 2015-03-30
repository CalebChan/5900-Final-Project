#pragma once



//=============================================================================
// meshSurface.h 
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





#ifndef NUSS_MESH_SURFACE 
#define NUSS_MESH_SURFACE 1


#include "gameobject.h"
#include "Shader.h"
//#include "camera.h"

#define NUSS_SHADERS 1


/***************************************************************************************/

// DATA STRUCTURES SECTION

struct meshVertex {
	float pos[4];
	float normal[4];	// the average normal 
	float texCoord1[2];
	float texCoord2[2];
	short numFaces;		// number of faces shared by the vertex not used
    float col[4];

};






class meshSurface : public gameObject
{
public:
	meshSurface(void);
	~meshSurface(void);
	meshSurface(int numRows, int numCols, float dx, float dz);
	  //virtual int render(int time);
	int meshSurface::render(int time, Matrix4f *worldTransformation, camera *cam);
	int meshSurface::render(Matrix4f *worldTransformation, camera *cam);
	int render(Matrix4f *worldMat, camera *cam, Matrix4f *otherMat, RENDER_MAT_TYPE type);
	int createSurface(int numRows, int numCols, float dx, float dz);
	int createGraphicsBuffers(Shader *shader);
//	int createGraphicsBuffers(void);
//	int createVAO(Shader *shader);
	int setCentrePosition(Vector3f centrePos);
	int setScale(float scaleX, float scaleY, float scaleZ);
	int loadTexture(char * fileName);


public:
	long *ind;				// holds the topolcoy (indices) of the geometry
	float dx;	// cell dimension along x axis
	float dz;	// cell dimension along z axis
		
	int * meshDecl;

	int mNumRows;
	int mNumCols;

	int mNumQuads;
	int mNumTriangles;
	GLuint *mIndBuf;
	GLuint mNumVtx;
	struct meshVertex *mVtxBuf;  // holds the vertices geometry
	GLuint mNumInd;

	GLuint mVtxVbo;
	GLuint mIndVbo;
	GLuint mVao;
	GLuint tex;		// the texture

private:
	void createQuad(GLuint *ind, int bottomLeft, int bottomRight, int topLeft, int topRight);
};



#endif