

//=============================================================================
// Chaser.cpp 
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



#include "Chaser.h"

#include "Constant.h"
#include <vector>
#include "gameApp.h"

#include <math.h>
#include <algorithm>
#include "House.h"
/**************************************************************************/

#define PI 3.14159265

Chaser::Chaser(void)
{
	this->hasSeenPrey = false;
	this->walkDist = 0;
	this->walkDir = Vector3f(rand() % 5 - 2, 0, rand() % 5 - 2);
	if (this->walkDir.length() == 0){
		this->walkDir = Vector3f(rand() % 5 - 2, 0, rand() % 5 - 2);
	}
}




/**************************************************************************/


Chaser::~Chaser(void)
{
}




/**************************************************************************/

// this is the object behaviour.  Here the logic should be  

int Chaser::updateState(int time) 	// update the object state

{
	
	/*float angle = 0.0f;
	angle = std::min((float)getAngle(), MAX_CAR_TURN_SPEED);
	int ret = this->yaw(angle);
	if (abs(angle) <= 0.001){
		this->moveForward(MAX_CAR_MOVE_SPEED);
		this->walkDist--;
	}
	//printf("Dist %d P.X %f P.Z %f Ang %f Real %f\n", walkDist, walkDir.x, walkDir.z, angle, (float)getAngle());

	std::vector<House *> gameEnt = gameApp::staticHouses;
	bool hasCollided = false;
	for (int i = 0; i < gameEnt.size(); i++){
		hasCollided |= this->hasCollision(gameEnt.at(i));
	}

	
	if (checkCrash()){
		gameApp::myApp->updateGameState(GAME_OVER);
		return 0;
	}

	if (hasCollided){
		//printf("Has Collided\n");
		this->moveForward(-MAX_CAR_MOVE_SPEED);
		this->yaw(-angle);
		randomWalk();
	}

	if (!targetWalk()){
		if (walkDist <= 0){
			randomWalk();
		}
	}*/

	return 0;
}

void Chaser::changeDest(Vector3f dir, int dist){
	walkDir = Vector3f(dir);

	if (dist != -1){
		walkDist = dist;
	}

	//printf("Change Dist %d P.X %f P.Z %f\n", walkDist, walkDir.x, walkDir.z);
}

bool Chaser::checkCrash(){
	Vector3f p = Vector3f(gameApp::preyRedKia->mPosition) - this->mPosition;
	if (p.length() < (3.5)){
		return true;
	}

	return false;
}

int Chaser::getAngle(void){

	Vector3f walk = Vector3f(this->walkDir);
	Vector3f look = Vector3f(this->lookAtVector);

	float dot = Vector3f::dot(walk, look);
	float total = dot / (walk.length() * look.length());

	return convertDotToAngle(total);
}

int Chaser::convertDotToAngle(float dot){
	float ang = 0;
	if (dot >= 0.999999){
		ang = 0;
	}
	else if (dot <= -0.999999){
		ang = 180;
	}
	else{
		ang = acos(dot)  * 180.0 / PI;
		
	}
	return ang;
}

bool Chaser::hasCollision(House *otherObject){
	Vector3f cHigh = this->getHighestVector();
	Vector3f cLow = this->getLowestVector();

	Vector3f oHigh = otherObject->getHighestVector();
	Vector3f oLow = otherObject->getLowestVector();

	printf("HX : %f HZ : %f\n", oHigh.x, oHigh.z);
	printf("LX : %f LZ : %f\n", oLow.x, oLow.z);

	if (cHigh.x < oLow.x || cHigh.z < oLow.z || cLow.x > oHigh.x || cLow.z > oHigh.z){
		return false;
	}
	return true;
	/*Vector3f pos = this->mPosition - otherObject->mPosition;
	if (pos.length() > 5.5){
		return false;
	}
	return true;*/
}

bool Chaser::targetWalk(void){
	Vector3f x2 = Vector3f(this->lookAtVector);
	Vector3f x0 = Vector3f(gameApp::preyRedKia->mPosition - this->mPosition);

	int angle = convertDotToAngle(Vector3f::dot(x2, x0) / (x2.length() * x0.length()));
	if (angle > 10){
		return false;
	}
	//printf("See Prey %d\n", angle);
	std::vector<House *> gameEnt = gameApp::staticHouses;
	bool hasCollided = false;
	for (int i = 0; i < gameEnt.size(); i++){
		if (intersectHouse(gameEnt.at(i))){
			return false;
		}
	}
	changeDest(Vector3f(gameApp::preyRedKia->mPosition - this->mPosition), ceil(x0.length() / MAX_CAR_MOVE_SPEED));
	
	return true;
}


bool Chaser::intersectHouse(House *other){
	Vector3f x1 = this->mPosition;
	Vector3f x2 = this->mPosition + this->lookAtVector;
	Vector3f x0 = other->mPosition;
	float num = abs((x2.z - x1.z) * x0.x - (x2.x - x1.x) * x0.z + x2.x * x1.z - x2.z * x1.x) / lookAtVector.length();

	return num <= 5.5;
}

void Chaser::randomWalk(){
	float ang;
	Vector3f newDir;
	if (this->walkDist <= 0){
		Vector3f newDir = Vector3f(rand() % 5 - 2, 0, rand() % 5 - 2);
		if (newDir.length() == 0){
			newDir = Vector3f(rand() % 5 - 2, 0, rand() % 5 - 2);
		}
		changeDest(newDir, rand() % 50);
	}
}

/*************************************************************************************/

/**************************************************************************/
// responds to a key stroke
int Chaser::processKeys(unsigned char key)
{

	return 0;
}

/**************************************************************************/
// responds to  seq. of key strokes
int Chaser::processKeys(unsigned char *key, int numKeys)

{

	return 0;
}

