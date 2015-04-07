#include "PointOfView.h"

#include <stdio.h>

#include "Constant.h"

#define TURN_SPEED 1

using namespace std;

Matrix4f PointOfView::getProjectionMatrix(Matrix4f *projMatrix)
{
	Matrix4f m;

	//m = Matrix4f::frustumProjectionMatrix(-0.025, -0.1, 0.025, 0.1, 0.01, 50);
	return Matrix4f::symmetricPerspectiveProjectionMatrix(60, 2, 0.1, 100);
	if (projMatrix != NULL) *projMatrix = m;

	return(m);
}

void PointOfView::updatePOVCamera(){
	/*this->moveForward(0.05);
	if (!isSet){
		return;
	}
	float angle = Vector3f::dot(position, nextLocation);
	if (angle >= 0){
		if (angle > TURN_SPEED){
			angle = TURN_SPEED;
		}
	}
	else{
		if (angle < -TURN_SPEED){
			angle = -TURN_SPEED;
		}
	}
	this->yaw(angle);
	
	printf(" ANGLE : %f\n", angle);
	isSet = false;*/
}

void PointOfView::setNextLocation(Vector3f nextLocation){
	this->nextLocation = Vector3f(nextLocation);
	isSet = true;
}