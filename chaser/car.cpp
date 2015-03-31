

//=============================================================================
// car.cpp 
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

#include "stdio.h"
#include <string.h>
#include "car.h"
//#include "nuss_util.h"
#include "SOIL.h"
#include <vector>

#include <algorithm>
using namespace std;
#include "gameApp.h"


//#include <stddef.h>     /* offsetof */




/******************************************************************/
/*
Purpose: car constructor


Descripton: 

Return:

*/

Car::Car(void)
: mNumVtx(0)
{

	setScale(1.0, 1.0, 1.0);
	this->setPositionOrientation(Vector3f(0,0,0),Vector3f(0,0,1),Vector3f(0,1,0));


}

/******************************************************************/
/*
Purpose: car destructor


Descripton: releases all allocated memory and com objects

Return:

*/

Car::~Car(void)
{
	// releae the com objects


	// free the memory


	
}

int Car::configureShader(Shader *shader){
	GLint shaderLoc;
	//copy the vertex position
	shaderLoc = glGetAttribLocation(shader->getProgId(), "vPos");
	glEnableVertexAttribArray(shaderLoc);
	glVertexAttribPointer(shaderLoc, 4, GL_FLOAT, GL_FALSE, sizeof(struct carVertex), (void*)offsetof(struct carVertex, pos));

	//copy the vertex colour
	shaderLoc = glGetAttribLocation(shader->getProgId(), "vColour");
	glEnableVertexAttribArray(shaderLoc);
	glVertexAttribPointer(shaderLoc, 4, GL_FLOAT, GL_FALSE, sizeof(struct carVertex), (void*)offsetof(struct carVertex, col));

	//copy the vertex normal
	shaderLoc = glGetAttribLocation(shader->getProgId(), "vNormal");
	glEnableVertexAttribArray(shaderLoc);
	glVertexAttribPointer(shaderLoc, 4, GL_FLOAT, GL_FALSE, sizeof(struct carVertex), (void*)offsetof(struct carVertex, normal));

	//copy the texture coordinates
	shaderLoc = glGetAttribLocation(shader->getProgId(), "vTex");
	glEnableVertexAttribArray(shaderLoc);
	glVertexAttribPointer(shaderLoc, 2, GL_FLOAT, GL_FALSE, sizeof(struct carVertex), (void*)offsetof(struct carVertex, texCoord1));

	return 0;
}

/******************************************************************/
/*
Purpose: creates the required index and vertex buffers on the graphics card


Descripton: 

Return:
0 if success

*/


int Car::createGraphicsBuffers(Shader *shader)
{
	int rc = 0;
	struct carVertex *v = NULL;
	long *ind = NULL;
	

	    //create vertex array object
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    //create vertex buffer object
    glGenBuffers(1, &mVtxVbo);
    glBindBuffer(GL_ARRAY_BUFFER, mVtxVbo);		
    glBufferData(GL_ARRAY_BUFFER, mNumVtx * sizeof(struct carVertex), mVtxBuf, GL_STATIC_DRAW);

	configureShader(shader);


    //end creation
    glBindVertexArray( 0 );




	// load the geometry


	return(rc);
}



/******************************************************************/
/*
Purpose: renders the object


Descripton: 

input:
time - game time
worldMat - for hierarchical objects
cam - the camera
Return:

*/


int Car::render(int time, Matrix4f *worldMat, camera *cam)
{

    static int angle = 0;
	static float step = 1;
	float rad = 0;
	static int i=0, j=0, k=0;
	Matrix4f modelWorldMat;  // model and world transformation. 
	Matrix4f modelMat, viewMat, projMat;
	
	glUseProgram(shader->getProgId());
	// set up the mode to wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// set the transformation of the object
	// initial model2World transformation - it aligh the object so that it faces the position z-axis

	modelMat= Matrix4f::rotateY(this->mYaw,1)*Matrix4f::scale(mScaleX, mScaleY, mScaleZ);
	modelWorldMat = Matrix4f::objectMatrix(this->mPosition, this->mPosition+this->lookAtVector,this->upVector)*modelMat;
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

	// transfer the texture handle to shader
//	glActiveTexture(GL_TEXTURE0 + 1);
//	glBindTexture(GL_TEXTURE_2D, this->tex);
	

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,tex);
	GLuint texLoc = glGetUniformLocation(this->shader->getProgId(), "texHandle");
    glUniform1i(texLoc, 3);
	GLint ttt = 0;
	glGetUniformiv(this->shader->getProgId(), texLoc, &ttt);


	// redner the triangles
	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0,mNumInd);
	//glDrawArrays(GL_QUADS, 0,mNumInd);

	//glDrawArrays(GL_TRIANGLES, mNumInd, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

   
	return 0;
}



/******************************************************************/
/*
Purpose: renders the object


Descripton: 

input:
worldMat - for hierarchical objects
cam - the camera
Return:

*/


int Car::render(Matrix4f *worldMat, camera *cam)
{
	return this->render(worldMat, cam, NULL, NORMAL);
}


int Car::render(Matrix4f *worldMat, camera *cam, Matrix4f *otherMat, RENDER_MAT_TYPE type){
	static int angle = 0;
	static float step = 1;
	float rad = 0;
	static int i = 0, j = 0, k = 0;
	Matrix4f modelWorldMat;  // model and world transformation. 
	Matrix4f modelMat, viewMat, projMat;

	glUseProgram(shader->getProgId());
	// set up the mode to wireframe

	// set the transformation of the object
	// initial model2World transformation - it aligh the object so that it faces the position z-axis

	modelMat = Matrix4f::rotateY(this->mYaw, 1)*Matrix4f::scale(mScaleX, mScaleY, mScaleZ);
	modelWorldMat = Matrix4f::objectMatrix(this->mPosition, this->mPosition + this->lookAtVector, this->upVector)*modelMat;
	//if (worldMat != NULL) modelWorldMat = *worldMat *  modelWorldMat;

	switch (type){
	case NORMAL:
	case DEPTH:
	{
		// set the camera position
		viewMat = cam->getViewMatrix(NULL);

		modelWorldMat = viewMat * modelWorldMat;
		// transfer to shader 
		shader->copyMatrixToShader(modelWorldMat, "modelWorldViewMat");

		// set the camera position
		projMat = cam->getProjectionMatrix(NULL);

		// transfer to shader 
		shader->copyMatrixToShader(projMat, "projMat");
		break;
	}
	case LIGHT:
	{
		viewMat = cam->getViewMatrix(NULL);

		modelWorldMat = viewMat * modelWorldMat;
		// transfer to shader 
		shader->copyMatrixToShader(modelWorldMat, "modelWorldViewMat");

		// set the camera position
		projMat = cam->getProjectionMatrix(NULL);

		// transfer to shader 
		shader->copyMatrixToShader(projMat, "projMat");
		//Matrix4f tmpMatrix = Matrix4f::identity() * *otherMat;
		shader->copyMatrixToShader(*otherMat, "biasMat");

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gameApp::terrainTexId);

		GLuint texLoc = glGetUniformLocation(this->shader->getProgId(), "shadowMap");
		glUniform1i(texLoc, 0);
		break;
	}
	}

	// transfer the texture handle to shader
	//	glActiveTexture(GL_TEXTURE0 + 1);
	//	glBindTexture(GL_TEXTURE_2D, this->tex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, tex);
	GLuint texLoc = glGetUniformLocation(this->shader->getProgId(), "texHandle");
	glUniform1i(texLoc, 3);
	GLint ttt = 0;
	glGetUniformiv(this->shader->getProgId(), texLoc, &ttt);

	// redner the triangles
	glBindVertexArray(mVao);
	glDrawArrays(GL_TRIANGLES, 0, mNumInd);
	glBindVertexArray(0);


	return 0;
}


/******************************************************************/
/*
Purpose: sets the surface scaling factor


Descripton: set the position of hte surfce.  The srufce should be 
translated so the the vertex at (0,?,0) is moved to 
Return:

*/





int Car::setScale(float scaleX, float scaleY, float scaleZ)
{
	this->mScaleX = scaleX;
	this->mScaleY = scaleY;
	this->mScaleZ = scaleZ;
	return 0;
}

/***************************************************************************************************/

// update hte indices of a quad

void Car::createQuad(GLuint *ind, int bottomLeft, int bottomRight, int topLeft, int topRight)
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


/***********************************************************************************************************/

#if 1
int Car::loadModelOBJ(char *fileName, struct carVertex **vtxBuf, GLuint *numVtx, GLuint **indBuf, GLuint *numInd)

{
	int rc = 0;
	int tmpIndex;	// a temporary index used for debbugingh pruposes.  It can be removed
	char line[1028];
	int count = 0;


	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3f> temp_vertices;
	std::vector<Vector2f> temp_uvs;
	std::vector<Vector3f> temp_normals;


	FILE * file = fopen(fileName, "r");
	if (file == NULL){
		printf("Error cannot open file %s \n", fileName);
		getchar();
		return 1;
	}

	while (!feof(file)){
		char lineHeader[128];		// assumes first word is shorter than 127 characters 
		char *s;					// position the head of the string after the lineHeader;
		int rc;

		count++;
		// get the first line
		if (fgets(line,1028,file) == NULL) continue;
//		else printf("line[%d]=%s \n",count,line);


		// else : parse lineHeader

		rc = sscanf(line,"%s",lineHeader);
		s = strchr(line, ' ');
//		printf("    s = %s\n",s);

		if (strcmp(lineHeader, "v") == 0){
			Vector3f vertex;
			sscanf(s, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0){
			Vector2f uv;
			sscanf(s, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0){
			Vector3f normal;
			sscanf(s, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			int i;
			unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			int matches = sscanf(s, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", 
				&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
				&vertexIndex[1], &uvIndex[1], &normalIndex[1], 
				&vertexIndex[2], &uvIndex[2], &normalIndex[2], 
				&vertexIndex[3], &uvIndex[3], &normalIndex[3]);
			if (matches != 9 && matches != 12) {
				//printf("face is not a triangle or a quad !!!\n");
				assert(matches != 9 && matches != 12);
			}
			for (i = 0; i < 3; i++) {
				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndices.push_back(normalIndex[i]);
			}
#if 1
			if (matches == 12) {
				// create a second triangle of the quad
				i = 2;
				vertexIndices.push_back(vertexIndex[i]); uvIndices.push_back(uvIndex[i]); normalIndices.push_back(normalIndex[i]);
				i = 3;
				vertexIndices.push_back(vertexIndex[i]); uvIndices.push_back(uvIndex[i]); normalIndices.push_back(normalIndex[i]);
				i = 0;
				vertexIndices.push_back(vertexIndex[i]); uvIndices.push_back(uvIndex[i]); normalIndices.push_back(normalIndex[i]);
			}
#endif

		}
		else{
			// Probably a comment, eat up the rest of the line
			continue;

		}

	}

	// the next code creates a triangle list using the vertices instead of indices.  
	// This is most likely done in order to capture the normals of each face.  

	// alocate memory for the indices.
	*numInd = vertexIndices.size();
	*indBuf = (GLuint *)malloc(sizeof(GLuint)* *numInd);
	if (*indBuf == NULL) {
		printf("errro in allocating memory for index buffer \n");
		rc = -1;
		goto err;
	}

	// allocate memory for the vertices
	*numVtx = vertexIndices.size();
	*vtxBuf = (struct carVertex *)malloc(sizeof(struct carVertex)* *numVtx);
	if (*vtxBuf == NULL) {
		printf("errro in allocating memory for the vertex buffer  \n");
		rc = -1;
		goto err;
	}

	// set the arrays to 0
	memset(*vtxBuf, 0, *numVtx * sizeof(struct carVertex));
	memset(*indBuf, 0, *numInd * sizeof(GLuint));


	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++){
		tmpIndex = vertexIndices[i] - 1;		//  current vtx indedx that is processed
		// populate the indices
		(*indBuf)[i] = tmpIndex;
		//printf("i = %d \n", i);


		// populate the verteices.  
		(*vtxBuf)[i].pos = Vector4f(temp_vertices[tmpIndex], 1);	// vtx geometry


		// add the texture coords
		tmpIndex = uvIndices[i] - 1;
		(*vtxBuf)[i].texCoord1 = temp_uvs[tmpIndex];

		// add the normal
		tmpIndex = normalIndices[i] - 1;
		(*vtxBuf)[i].normal = Vector4f(temp_normals[tmpIndex], 1);
		(*vtxBuf)[i].col = Vector4f(1,0,0,1);
		struct carVertex v = (*vtxBuf)[i];
		v.col.x +=0;



	}

	this->createGraphicsBuffers(shader);

err:

	return 0;
}


#endif


/**************************************************************************************/
//  load the texture Define the geometry
int Car::loadTexture(char * fileName)
{


	int width=512, height=512;
    unsigned char* image;
	int channels = 0;

	// generate the texture
	glGenTextures(1, &this->tex);

	glActiveTexture(GL_TEXTURE0 + 1);

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



/**************************************************************************/

// this is the object behaviour.  Here the logic should be  

int Car::updateState(int time) 	// update the object state

{
	static int dir = 1;

	return 0;
}




/**************************************************************************/
// responds to a key stroke
int Car::processKeys(unsigned char key)
{

	return 0;
}

/**************************************************************************/
// responds to  seq. of key strokes
int Car::processKeys(unsigned char *key, int numKeys)

{

	return 0;
}


Vector3f Car::getHighestVector(){
	Matrix4f m = getModelMatrix(NULL);
	Matrix4f m1 = Matrix4f::scale(mScaleX, mScaleY, mScaleZ);
	m = m * m1;
	float hX = -999;
	float hY = -999;
	float hZ = -999;
	for (int i = 0; i < mNumVtx; i++){
		carVertex v = mVtxBuf[i];
		Vector4f pos = m * v.pos;
		hX = max(pos.x, hX);
		hY = max(pos.y, hY);
		hZ = max(pos.z, hZ);
	}

	return Vector3f(hX, hY, hZ);
}

Vector3f Car::getLowestVector(){
	Matrix4f m = getModelMatrix(NULL);
	Matrix4f m1 = Matrix4f::scale(mScaleX, mScaleY, mScaleZ);
	m = m * m1;
	float hX = 999;
	float hY = 999;
	float hZ = 999;
	for (int i = 0; i < mNumVtx; i++){
		carVertex v = mVtxBuf[i];
		Vector4f pos = m * v.pos;
		hX = min(pos.x, hX);
		hY = min(pos.y, hY);
		hZ = min(pos.z, hZ);
	}

	return Vector3f(hX, hY, hZ);
}


float Car::getRadius(){
	Vector3f high = getHighestVector() - this->mPosition;
	Vector3f low = getLowestVector() - this->mPosition;
	return max(high.length(), low.length());
}


