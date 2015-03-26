#pragma once




//=============================================================================
// Chaser.h 
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. 
// b.  
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













#include "car.h"

#include "House.h"


class Chaser : public Car
{
public:
	Chaser(void);
	~Chaser(void);

	virtual int updateState(int time); 	// update the object state
	virtual int processKeys(unsigned char key); // responds to a key stroke
	virtual int processKeys(unsigned char *key, int numKeys); // responds to  seq. of key strokes


private:
	Vector3f walkDir;
	int walkDist;
	bool hasSeenPrey;

	static bool compareVector(Vector3f v1, Vector3f v2);

	void randomWalk(void);
	bool targetWalk(void);

	bool hasCollision(House *otherObject);
	int getAngle(void);

	bool intersectHouse(House *other);
	int convertDotToAngle(float dot);

	bool checkCrash();

	void changeDest(Vector3f dir, int dist);
};

