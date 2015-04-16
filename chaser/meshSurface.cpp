

//=============================================================================
// meshSurface.cpp 
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



#include "meshSurface.h"
//#include "nuss_util.h"
#include "SOIL.h"
#include "gameApp.h"
//#include <stddef.h>     /* offsetof */




/******************************************************************/
/*
Purpose: meshSurface constructor


Descripton: 

Return:

*/

meshSurface::meshSurface(void)
: ind(0), 
  mNumVtx(0), 
  mNumQuads(0), 
  mNumTriangles(0), 
  mNumRows(0),
  mNumCols(0)
{

	setScale(1.0, 1.0, 1.0);
	this->setCentrePosition(Vector3f(0.0,0.0,0.0));


}

/******************************************************************/
/*
Purpose: meshSurface destructor


Descripton: releases all allocated memory and com objects

Return:

*/

meshSurface::~meshSurface(void)
{
	// releae the com objects


	// free the memory


	
}


/******************************************************************/
/*
Purpose: meshSurface constructor


Descripton: creates a surface with numRows x NumCols vertices and (numRows-1) x (NumCols-1)
quads.  Each quad size is dx x dz.  

Note that the y axis represents the height and the mesh spans the x-z axis.

Return:

*/
meshSurface::meshSurface(int numRows, int numCols, float dx, float dz) 
: ind(0), 
  mNumVtx(0), 
  mNumQuads(0), 
  mNumTriangles(0), 
  mNumRows(0),
  mNumCols(0)
{

	setScale(1.0, 1.0, 1.0);
	createSurface(numRows, numCols, dx, dz);



}


/**************************************************************************************/
/* this function creates a surface.  The sureface is genreated using two arrays.  The vertex data array which 
contains the vertex data (geometry) and an index array which contains the topology of the triangles.  The trainagles 
are stored in the index array as a triangle list.  

Input
numCols - number of columns.  For example if numLong == 10 than the surface will have 10 columns

numRows - number rows. 

height - the height of the surface.  Can beachieved by scaling the surface.

width - the width of the surface.  Can beachieved by scaling the surface.

Ouptut:
vtx - a buffer with all the vertex information.  Currently the function computes the position information
and the normal of each vertex.  Note, the array is allocted by the function.

numVtx1 - returns the number of vertices there were genrated.

ind - a buffer which contains the topology of the triangles.

numInd 1 - the number of entries in the buffer. 

Return:
the function returns 0 is successful.


*/

int meshSurface::createSurface(int numSurfaceRows, int numSurfaceCols, float height, float width) 

{	
	int rc = 0;
	int i,j,k;
	float dh = 0.0;	// angle of latitude starting from the "south pole" at angle -90
	float dw = 0.0;   // angle of longtitude in the rage of 0-360
	float deltaHeight;	// the increment of the surface along the rows
	float deltaWidth;	// the increment of the surface along the columns
	float dTexX, dTexY;	// delta for setting up the texture coordinates
	float colour[3][4] = {{1.0, 0.0, 0.0, 1.0},
						  {0.0, 1.0, 0.0, 1.0},
						  {0.0, 0.0, 1.0, 1.0}};

	
	//int numTriangles;



	this->mNumRows = numSurfaceRows;  // number of horizonal slabs
	this->mNumCols = numSurfaceCols;	// number of vertical slabs

	mNumVtx = (mNumRows+1) * (mNumCols+1);   // define the number of required vertices

	mNumQuads = mNumRows * mNumCols;
	mNumTriangles = mNumQuads * 2;	

	// allocate memory
	mVtxBuf = (struct meshVertex *) malloc(sizeof(struct meshVertex) * mNumVtx);
	if (mVtxBuf == NULL) {
		// error
		rc = 1;
		goto err;
	}

	this->mNumInd = mNumTriangles*3;
	mIndBuf = (GLuint *) malloc(sizeof(GLuint) * this->mNumInd);
	if (mIndBuf == NULL) {
		// error
		this->mNumInd = 0;
		rc = 1;
		goto err;
	}

	// Fill the vertex buffer with positions
	k = 0;
	dw = dh = 0.0;  
	float textureRrepeatFactor = (float) 1;
	dTexX = (float) 1.0/(textureRrepeatFactor*mNumCols);
	dTexY = (float) 1.0/(textureRrepeatFactor*mNumRows);
	deltaHeight = (float)height / mNumRows; // increment of height
	deltaWidth = (float)width / mNumCols; // increment of width

	for(i = 0, dh = -height/ (float) 2.0; i <= mNumRows; i++ ,dh += deltaHeight) {
		for(j = 0, dw = -width/ (float) 2.0; j <= mNumCols; j++, dw += deltaWidth) {
			mVtxBuf[k].normal[0]=0;
			mVtxBuf[k].normal[1]=1;
			mVtxBuf[k].normal[2]=0;
			mVtxBuf[k].normal[3]=0;

			// position in space 
			mVtxBuf[k].pos[0]  = dw;
			mVtxBuf[k].pos[1]  = 0;
			mVtxBuf[k].pos[2]  = dh;
			mVtxBuf[k].pos[3]  = 1;

			memcpy((void *)mVtxBuf[k].col, (void *)colour[k%3], sizeof(colour[1])); 

			mVtxBuf[k].texCoord1[0]  = j*dTexX;
			mVtxBuf[k].texCoord1[1]  = i*dTexY;

			

			struct meshVertex v;
			v = mVtxBuf[k];

			k++;
		}
	}

	


	// fill the index buffer

	k = 0;
	for(i = 0; i < mNumRows; i++) {
		for(j = 0; j < mNumCols; j++)
		{
			// fill indices for the quad
			// change by making a quad function
			createQuad(&mIndBuf[k], i * (mNumCols+1) + j, i * (mNumCols+1) + j + 1, (i+1) * (mNumCols+1) + j, (i+1) * (mNumCols+1) + j + 1);

			k+=6;
		}
	}



	this->createGraphicsBuffers(shader);
//	this->createGraphicsBuffers();
//	this->createVAO(shader);



	return(0);
err:
	return(rc);
}

#if 1
/******************************************************************/
/*
Purpose: creates the required index and vertex buffers on the graphics card


Descripton: 

Return:
0 if success

*/


int meshSurface::createGraphicsBuffers(Shader *shader)
{
	int rc = 0;
	struct meshVertex *v = NULL;
	long *ind = NULL;
	GLint shaderLoc;

    //create vertex array object
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    //create vertex buffer object
    glGenBuffers(1, &mVtxVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVtxVbo);		
    glBufferData(GL_ARRAY_BUFFER, mNumVtx * sizeof(struct meshVertex), mVtxBuf, GL_STATIC_DRAW);


    //copy the vertex position
	shaderLoc = glGetAttribLocation(shader->getProgId(), "vPos");
	//shaderLoc = glGetAttribLocation(classShader->getProgId(), "vPos");
    glEnableVertexAttribArray(shaderLoc);
    glVertexAttribPointer(shaderLoc, 4, GL_FLOAT, GL_FALSE, sizeof(struct meshVertex), (void*) offsetof(struct meshVertex, pos));
 
	//copy the vertex colour
	shaderLoc = glGetAttribLocation(shader->getProgId(), "vColour");
//	shaderLoc = glGetAttribLocation(classShader->getProgId(), "vColour");
    glEnableVertexAttribArray(shaderLoc);
    glVertexAttribPointer(shaderLoc, 4, GL_FLOAT, GL_FALSE, sizeof(struct meshVertex), (void*) offsetof(struct meshVertex, col));

    //copy the vertex normal
	shaderLoc = glGetAttribLocation(shader->getProgId(), "vNormal");
//	shaderLoc = glGetAttribLocation(classShader->getProgId(), "vNormal");
	glEnableVertexAttribArray(shaderLoc);
    glVertexAttribPointer(shaderLoc, 4, GL_FLOAT, GL_FALSE, sizeof(struct meshVertex), (void*) offsetof(struct meshVertex, normal));

    //copy the texture coordinates
	shaderLoc = glGetAttribLocation(shader->getProgId(), "vTex");
//	shaderLoc = glGetAttribLocation(classShader->getProgId(), "vTex");
    glEnableVertexAttribArray(shaderLoc);
    glVertexAttribPointer(shaderLoc, 2, GL_FLOAT, GL_FALSE, sizeof(struct meshVertex), (void*) offsetof(struct meshVertex, texCoord1));

    //create index buffer
    glGenBuffers( 1, &mIndVbo);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIndVbo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, mNumInd * sizeof(GLuint), mIndBuf, GL_STATIC_DRAW );

    //end creation
    glBindVertexArray( 0 );




	// load the geometry


	return(rc);
}
#endif

//#include "pieces.cpp"

/******************************************************************/
/*
Purpose: change the position of the sufce.


Descripton: set the position of hte surfce.  The srufce should be 
translated so the the vertex at (0,?,0) is moved to 
Return:

*/


int meshSurface::setCentrePosition(Vector3f centrePos)
{
	// to be udpated 
	this->mPosition = centrePos;
	return 0;
}


/******************************************************************/
/*
Purpose: renders the surface


Descripton: 

input:
time - game time
worldMat - for hierarchical objects
cam - the camera
Return:

*/


int meshSurface::render(int time, Matrix4f *worldMat, camera *cam)
{

    static int angle = 0;
	static float step = 1;
	float rad = 0;
	static int i=0, j=0, k=0;
	Matrix4f modelWorldMat;  // model and world transformation. 
	Matrix4f viewMat, projMat;
	
	glUseProgram(shader->getProgId());
		// set up the mode to wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// set the transformation of the object
	modelWorldMat =  Matrix4f::translation(this->mPosition)*Matrix4f::scale(mScaleX, mScaleY, mScaleZ);
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

//	glActiveTexture(GL_TEXTURE0 + 2);
	//glBindTexture(GL_TEXTURE_2D, this->tex);
	// redner the triangles
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,tex);
	GLuint texLoc = glGetUniformLocation(this->shader->getProgId(), "texHandle");
    glUniform1i(texLoc, 3);
	GLint ttt = 0;
	glGetUniformiv(this->shader->getProgId(), texLoc, &ttt);

	glBindVertexArray(mVao);
    glDrawElements(GL_TRIANGLES, mNumInd, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);

   
	return 0;
}



/******************************************************************/
/*
Purpose: renders the surface


Descripton: 

input:
time - game time
worldMat - for hierarchical objects
cam - the camera
Return:

*/


int meshSurface::render(Matrix4f *worldMat, camera *cam)
{
	return render(worldMat, cam, NULL, NORMAL);
}

int meshSurface::render(Matrix4f *worldMat, camera *cam, Matrix4f *otherMat, RENDER_MAT_TYPE type){
	Matrix4f modelWorldMat;  // model and world transformation. 
	modelWorldMat = Matrix4f::translation(this->mPosition)*Matrix4f::scale(mScaleX, mScaleY, mScaleZ);

	Matrix4f bias = (otherMat == NULL) ? Matrix4f::identity() : *otherMat;

	renderShaderSetup(modelWorldMat, cam->getViewMatrix(NULL), cam->getProjectionMatrix(NULL), bias, gameApp::terrainTexId, type);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// redner the triangles
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, tex);
	GLuint texLoc = glGetUniformLocation(this->shader->getProgId(), "texHandle");
	glUniform1i(texLoc, 3);

	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, mNumInd, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	return 0;
}


/******************************************************************/
/*
Purpose: sets the surface scaling factor


Descripton: set the position of hte surfce.  The srufce should be 
translated so the the vertex at (0,?,0) is moved to 
Return:

*/





int meshSurface::setScale(float scaleX, float scaleY, float scaleZ)
{
	this->mScaleX = scaleX;
	this->mScaleY = scaleY;
	this->mScaleZ = scaleZ;
	return 0;
}

/***************************************************************************************************/

// update hte indices of a quad

void meshSurface::createQuad(GLuint *ind, int bottomLeft, int bottomRight, int topLeft, int topRight)
{
	int k = 0;

		ind[k] = bottomLeft;
		k++;
		ind[k] = bottomRight; 
		k++;
		ind[k] = topRight;
		k++;
		ind[k] = topRight;
		k++;
		ind[k] = topLeft;
		k++;
		ind[k] = bottomLeft;
}


/**************************************************************************************/
//  load the texture Define the geometry
int meshSurface::loadTexture(char * fileName)
{


	int width=512, height=512;
    unsigned char* image;
	int channels = 0;

	// generate the texture
	glGenTextures(1, &this->tex);

	//glActiveTexture(GL_TEXTURE0 + 2);

	glBindTexture(GL_TEXTURE_2D,tex);


	GLubyte negZ[4] = {255, 255, 0, 1};

    image = SOIL_load_image(fileName, &width, &height, &channels, SOIL_LOAD_RGBA);
	//*tex = SOIL_load_OGL_texture(imageFileName, SOIL_LOAD_AUTO,	SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
	

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D,0);

//	loadSkyboxTexturesColour512(&texCube, cubeImageFileName);

	return(0);

}

