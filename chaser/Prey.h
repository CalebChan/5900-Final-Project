#pragma once



//=============================================================================
// Prey.h
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

class Prey : public Car
{
public:
	Prey(void);
	~Prey(void);

	virtual int updateState(int time); 	// update the object state
	virtual int processKeys(unsigned char key); // responds to a key stroke
	virtual int processKeys(unsigned char *key, int numKeys); // responds to  seq. of key strokes

	bool hasCollision(House *otherObject);

	void updatePrey(float move, float yaw);

private:
	float moveDx;
	float yawDx;
};

