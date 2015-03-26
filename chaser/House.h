#pragma once
#include "car.h"
class House :
	public Car
{
public:
	House(void);
	~House(void);

	Vector3f high;
	bool highSet = false;
	Vector3f low;
	bool lowSet = false;

	virtual Vector3f getHighestVector();
	virtual Vector3f getLowestVector();

	void recalculateVector();
};

