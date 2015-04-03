//=============================================================================
// gameObtjec.h 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. handles the common properties and functios of a game object 

//
// Description:
//--------------

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


#include "gameObject.h"

/**************************************************************************/
// static members

Shader *gameObject::classShader = NULL;


/**************************************************************************/

gameObject::gameObject(void):mRoll(0.0),mPitch(0.0),mYaw(0.0),
mPosition(0.0, 0.0, 0.0), lookAtVector(0.0, 0.0, 0.0), shader(NULL), totalYaw(0.0)
{
	
}

/**************************************************************************/

gameObject::~gameObject(void)
{
}

/**************************************************************************/

int gameObject::initGeom(void)
{
	return 0;
}

/**************************************************************************/
// initialize the geometry in a mesh file
int gameObject::initGeom(char *directory, char *meshFileName)
{
	return 0;
}

/**************************************************************************/

int gameObject::render(int time)
{

	return 0;
}


/**************************************************************************/

int  gameObject::render(int time, Matrix4f *worldTransformation, camera *cam)
{

	return 0;
}

int  gameObject::render(Matrix4f *worldMat, camera *cam, Matrix4f *otherMat, RENDER_MAT_TYPE type)
{

	return render(worldMat, cam);
}



/**************************************************************************/
#if 0
int  gameObject::render(Matrix4f *worldTransformation, camera *cam)
{

	return 0;
}

#endif
/**************************************************************************/


int gameObject::updateState(int time) 	// update the object state

{

	return 0;
}


/**************************************************************************/
// responds to a key stroke
int gameObject::processKeys(unsigned char key)
{

	return 0;
}

/**************************************************************************/
// responds to  seq. of key strokes
int gameObject::processKeys(unsigned char *key, int numKeys)

{

	return 0;
}



/**************************************************************************/
// set a shader id for the object from an external source

int gameObject::setShader(Shader *shader)
{
	this->shader = shader;
	return 0;
}

/**************************************************************************/
// retrieve the shader id of the object

Shader * gameObject::getShader(void)
{
	return(shader);
}


/**************************************************************************/
// set the shader id from an external source

int gameObject::setClassShader(Shader *shaderId)
{
	classShader = shaderId;
	return 0;
}

/**************************************************************************/
// retrieve the shader

Shader *gameObject::getClassShader(void)
{
	return(classShader);
}



/**************************************************************************/
// creates a shader id for the object
// a generaic function for creating a specific shader for a sub class object

int gameObject::createShaderProg(char* vsFileName, char* fsFileName)	

{
	GLuint shaderId;
	int rc = 0;

	shader = new Shader();
	if (this->shader == NULL) {
		printf("error in creating a shader obeject \n");
		assert(0);
	}
	rc = shader->createShaderProgram(vsFileName, fsFileName, &shaderId);
	if (rc != 0) {
		printf("error in generating shader vs=%s, fs=%s \n", vsFileName, fsFileName);
		delete shader;
		shader = NULL;
		assert(0);
	}
	shader->shaderProgramid = shaderId;

	return(rc);
}

/**************************************************************************/
// creates a shader id for the object
// a generaic function for creating a specific shader for a sub class object

int gameObject::createClassShaderProg(char* vsFileName, char* fsFileName)	

{
	GLuint shaderId;
	int rc = 0;

	this->classShader = new Shader();
	if (this->classShader == NULL) {
		printf("error in creating a shader obeject \n");
		assert(0);
	}
	rc = this->classShader->createShaderProgram(vsFileName, fsFileName, &shaderId);
	if (rc != 0) {
		printf("error in generating shader vs=%s, fs=%s \n", vsFileName, fsFileName);
		delete classShader;
		classShader = NULL;
		assert(0);
	}

	classShader->shaderProgramid = shaderId;
	
	return(rc);
}





/******************************************************************/
/*
Purpose: changes the object's position relative to its current position


Descripton: 

Return:
0 - success

*/


int gameObject::changePositionDelta(float dx, float dy, float dz)
{
	mPosition.x += dx;
	mPosition.y += dy;
	mPosition.z += dz;

	return 0;
}


/******************************************************************/
/*
Purpose: changes the object's position relative to its current position


Descripton: 

Return:
0 - success

*/


int gameObject::changePositionDelta(Vector3f *dv)
{
	if (dv != NULL) changePositionDelta(dv->x, dv->y, dv->z);

	return(0);
}

/******************************************************************/
/*
Purpose: changes the camera's position to a new position


Descripton: 

Return:
0 - success

*/


int gameObject::changeAbsPoition(float x, float y, float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
	return 0;
}

/******************************************************************/
/*
Purpose: changes the object's position to a new position


Descripton: 

Return:
0 - success

*/

int gameObject::changeAbsPosition(Vector3f *v)
{
	if (v != NULL) mPosition = *v;
	return 0;
}

/**************************************************************************/

int gameObject::changeAbsPosition(Vector3f v)
{
	mPosition = v;
	return 0;
}

/******************************************************************/
/*
Purpose: changes the object's position along the LootAt vector


Descripton: move the object forward by the numUnits along the lookAtVector vector

Return:
the new position

*/


Vector3f gameObject::moveForward(float numUnits)
{
	mPosition += lookAtVector *numUnits;

	return (Vector3f(mPosition.x, mPosition.y, mPosition.z));
}

/******************************************************************/
/*
Purpose: update the orientation of the axis.  It is done once for all rorations


Descripton: 

Return:


*/


int gameObject::updateOrientation(Vector3f rotVector, float angleRad)
{

	Vector3f xaxis(0.0,0.0,0.0);
	Vector4f v;

	// create rotation matrix
	rotMat = Matrix4f::rotateVector(rotVector, angleRad, 0);
	// rotate the camera (up vector and/or looAtVector)
	v = Vector4f(upVector, 1.0);
	v = rotMat*v;
	upVector.x = v.x;
	upVector.y = v.y;
	upVector.z = v.z;

	v = Vector4f(lookAtVector, 1.0);
	v = rotMat*v;
	lookAtVector.x = v.x;
	lookAtVector.y = v.y;
	lookAtVector.z = v.z;

	// update the upVector
	xaxis = Vector3f::cross(upVector, lookAtVector);
	upVector = Vector3f::cross(lookAtVector, xaxis);
	upVector.normalize();
	lookAtVector.normalize();
	return 0;
}


/**********************************************************************************/

/*
Purpose: set the object's position and orientation 


Descripton: 

Return:


*/


void gameObject::setPositionOrientation(Vector4f position, Vector4f lookAtPoint, Vector4f upVector)
{
	this->mPosition = Vector3f(position.x, position.y, position.z);
	this->lookAtVector = Vector3f(lookAtPoint.x, lookAtPoint.y, lookAtPoint.z)-this->mPosition;
	this->upVector =  Vector3f(upVector.x, upVector.y, upVector.z);
	this->upVector.normalize();
	this->lookAtVector.normalize();

}


/**************************************************************************************************/
// set the object position and orientation

void gameObject::setPositionOrientation(Vector3f position, Vector3f lookAtVector, Vector3f upVector)
{
	this->mPosition = position;
	this->lookAtVector = lookAtVector;
	this->upVector = upVector;
	this->upVector.normalize();
	this->lookAtVector.normalize();

}

/**************************************************************************************************/
// set the object position and orientation

void gameObject::setPositionLookAtPoint(Vector3f position, Vector3f lookAtPoint, Vector3f upVector)
{
	this->mPosition = position;
	this->lookAtVector = lookAtPoint - position;
	this->upVector = upVector;
	this->upVector.normalize();
	this->lookAtVector.normalize();

}

/*****************************************************************/
// change the speed of the object motion

int gameObject::updateSpeed(float DeltaSpeed)
{
	this->speed += DeltaSpeed;
	return 0;
}

/*****************************************************************/
// set the speed of the object motion

int gameObject::setSpeed(float speed)
{
	this->speed = speed;
	return 0;
}

/**************************************************************************************************/
// return the moving speed

float gameObject::getSpeed(void)
{
	return(speed);
}


/**************************************************************************************************/
// modify the lookat vector

int gameObject::changeLookAtVector(float x, float y, float z)
{
	lookAtVector.x = x;
	lookAtVector.y = y;
	lookAtVector.z = z;

	return 0;
}




/******************************************************************/
/*
Purpose: obtains the view transformation matrix


Descripton: 

Return:
the transformation matrix

*/



Matrix4f gameObject::getModelMatrix(Matrix4f *modelMatrix)
{
	Vector3f lookAtPoint;
	Matrix4f m;

	lookAtPoint = mPosition+lookAtVector;
	m = Matrix4f::cameraMatrix(mPosition, lookAtPoint, upVector);
	if (modelMatrix != NULL) *modelMatrix = m;

	return(m);
}



/******************************************************************/
/*
Purpose: change the orientation of the object (roll transformation) along the z axis


Descripton: 

Return:
1 - if failed
0 - if successful

*/


int gameObject::roll(float angleDeg)
{
	float angleRad = DegreeToRadians(angleDeg);
	Vector3f rotVector(0.0,0.0,0.0);


	// get rotation axis
	rotVector = lookAtVector;

	updateOrientation(rotVector, angleRad);


	return 0;
}


/******************************************************************/
/*
Purpose: change the orientation of the object (pitch transformation) along the x-axis


Descripton: 

Return:
1 - if failed
0 - if successful

*/

int gameObject::pitch(float angleDeg)
{
	float angleRad = DegreeToRadians(angleDeg);
	Vector3f rotVector(0.0,0.0,0.0);

	// get rotation axis
	rotVector = Vector3f::cross(lookAtVector, upVector);

	updateOrientation(rotVector, angleRad);

	return 0;
}

/******************************************************************/
/*
Purpose: change the orientation of the object (yaw transformation) along the y-axis


Descripton: 

Return:
1 - if failed
0 - if successful

*/



int gameObject::yaw(float angleDeg)
{
	this->totalYaw += angleDeg;
	float angleRad = DegreeToRadians(angleDeg);
	Vector3f rotVector(0.0,0.0,0.0);

	// get rotation axis
	rotVector = upVector;

	updateOrientation(rotVector, angleRad);


	return 0;
}



void gameObject::renderShaderSetup(Matrix4f model, Matrix4f view, Matrix4f proj, Matrix4f bias, GLuint shadowTexture, RENDER_MAT_TYPE type){
	glUseProgram(shader->getProgId());

	//model = view * model;
	// transfer to shader 
	shader->copyMatrixToShader(model, "modelWorldViewMat");
	// transfer to shader 
	shader->copyMatrixToShader(proj * view, "projMat");

	switch (type){
	case SHADOW:
	{
		//Matrix4f tmpMatrix = Matrix4f::identity() * *otherMat;
		shader->copyMatrixToShader(bias, "lightPVMat");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowTexture);

		GLuint texLoc = glGetUniformLocation(this->shader->getProgId(), "shadowMap");
		glUniform1i(texLoc, 0);
		break;
	}
	}
}