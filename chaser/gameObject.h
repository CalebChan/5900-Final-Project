#pragma once

#include "glew.h"
#include "glut.h"
#include "gl/gl.h"
#include "nuss_matrix.h"
#include "camera.h"
#include "shader.h"


/**************************************************************************/

// DEFINE
#define GAME_OBJECT_DELETE_ENTITY 10

/**************************************************************************/

enum RENDER_MAT_TYPE{
	NORMAL,
	DEPTH,
	LIGHT,
};

class gameObject
{
	// functions
public:
	gameObject(void);
	~gameObject(void);
	virtual int initGeom(void);
	virtual int initGeom(char *directory, char * meshFileName);
	virtual int render(int time);
	virtual int render(int time, Matrix4f *worldTransformation, camera *cam);
	virtual int render(Matrix4f *worldMat, camera *cam, Matrix4f *otherMat, RENDER_MAT_TYPE type);
	virtual int render(Matrix4f *worldTransformation, camera *cam) = 0;
	virtual int updateState(int time); 	// update the object state
	virtual int processKeys(unsigned char key); // responds to a key stroke
	virtual int processKeys(unsigned char *key, int numKeys); // responds to  seq. of key strokes

	// shader functions
	virtual int createShaderProg(char *vsFileName, char *fsFileName);	// a generaic function for creating a specifi shader for a sub class
	virtual int createClassShaderProg(char *vsFileName, char *fsFileName);	// a generaic function for creating a specifi shader for a sub class
	int setShader(Shader *shader);		// stores a generic shader id from an external source
	Shader * getShader(void);				// retrieves the shader handle
	int setClassShader(Shader *shader);		// stores a generic shader i from an external source
	Shader * getClassShader(void);			// retrieves the shader id from an external source

	// motion control functions

	// position
	int changePositionDelta(float dx, float dy, float dz);	// change position by relative amount
	int changePositionDelta(Vector3f *dv);				// change position by relative amount
	int changeAbsPoition(float x, float y, float z);		// change to a new position in space
	int changeAbsPosition(Vector3f *v);					// change to a new position in space
	int changeAbsPosition(Vector3f v);					// change to a new position in space

	// Move function
	Vector3f moveForward(float numUnits);		// moves forward by the numUnits units along the lookAtVector
	int updateSpeed(float DeltaSpeed);			// change the speed of the camera motion by a delta
	float getSpeed(void);						// get the speed of the object
	int setSpeed(float speed);					// set the speed of the object

	// change orientation 
	int roll(float angleDeg);
	int pitch(float angleDeg);
	int yaw(float angleDeg);

	
	// orientation 
	void setPositionOrientation(Vector3f position, Vector3f lookAtPoint, Vector3f upVector);
	void setPositionLookAtPoint(Vector3f position, Vector3f lookAtPoint, Vector3f upVector);
	void setPositionOrientation(Vector4f position, Vector4f lookAtPoint, Vector4f upVector);
	int changeLookAtVector(float x, float y, float z);  	
	int setNewLookAtPoint(float x, float y, float z);  	

	// model matrix

	Matrix4f getModelMatrix(Matrix4f * viewMatrix);


	//members
public:
	
	Vector3f mPosition;	// position of object
//	Vector3f mDir;		// dirction of object
	float mRoll;		// angle of rotation around z-axis
	float mPitch;		// angle of rotation around x-axis
	float mYaw;			// angle of rotation around y-axis
	float mScaleX;		// scale in x
	float mScaleY;		// scale in x
	float mScaleZ;		// scale in x
	Matrix4f rotMat;
	Vector3f upVector;
	Vector3f lookAtVector;  //  dirction of object
	float speed;


	float totalYaw;



protected:
	Shader *shader;	// the shader id that is associated with the object.  Note that this is not a class shader
	static Shader *classShader;	// shader id for the class;


private:
	int updateOrientation(Vector3f rotVector, float angleRad); // update the camera's orientation in space



protected:
	void renderShaderSetup(Matrix4f model, Matrix4f view, Matrix4f proj, Matrix4f bias, GLuint shadowTexture, RENDER_MAT_TYPE type);
};