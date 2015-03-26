#include "Prey.h"

#include "gameApp.h"
#include "House.h"

#include "Constant.h"
/**************************************************************************/


Prey::Prey(void)
{
}




/**************************************************************************/


Prey::~Prey(void)
{
}




/**************************************************************************/

// this is the object behaviour.  Here the logic should be  

int Prey::updateState(int time) 	// update the object state

{
	std::vector<House *> house = gameApp::staticHouses;
	//printf("Update %d\n", house.size());
	bool collide = false;
	for (int i = 0; i < house.size(); i++){
		if (hasCollision(house.at(i))){
			collide = true;
		}
	}

	if (hasCollision(gameApp::targetHouse)){
		gameApp::myApp->updateGameState(GAME_WON);
		return 0;
	}

	if (collide){
		this->moveForward(-moveDx);
		this->yaw(-yawDx);
		gameApp::updateFirstPersonCamera();
	}
	this->moveDx = 0;
	this->yawDx = 0;
	return 0;
}

void Prey::updatePrey(float move, float yaw){
	moveDx += move;
	yawDx += yaw;
}


bool Prey::hasCollision(House *otherObject){
	/*Vector3f cHigh = this->getHighestVector();
	Vector3f cLow = this->getLowestVector();

	Vector3f oHigh = otherObject->getHighestVector();
	Vector3f oLow = otherObject->getLowestVector();

	if (cHigh.x < oLow.x || cHigh.z < oLow.z || cLow.x > oHigh.x || cLow.z > oHigh.z){
		return false;
	}
	*/

	Vector3f pos = this->mPosition - otherObject->mPosition;
	//printf("Length %f Rad P: %f Rad H : %f\n", pos.length(), this->getRadius(), otherObject->getRadius());
	if (pos.length() > 5.5){
		return false;
	}
	return true;
}




/**************************************************************************/
// responds to a key stroke
int Prey::processKeys(unsigned char key)
{

	return 0;
}

/**************************************************************************/
// responds to  seq. of key strokes
int Prey::processKeys(unsigned char *key, int numKeys)

{

	return 0;
}
