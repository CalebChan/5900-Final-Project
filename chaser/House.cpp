#include "House.h"

House::House(void)
{
}


House::~House(void)
{
}


Vector3f House::getHighestVector(){
	if (!highSet){
		highSet = true;
		high = Car::getHighestVector();
		printf("H.X %f H.Y %f H.Z %f\n", high.x, high.y, high.z);
	}
	return high;
}
Vector3f House::getLowestVector(){
	if (!lowSet){
		lowSet = true;
		low = Car::getLowestVector();
		printf("L.X %f L.Y %f L.Z %f\n", low.x, low.y, low.z);
	}
	return low;
}

void House::recalculateVector(){
	highSet = false;
	lowSet = false; 
	getHighestVector();
	getLowestVector();
}

