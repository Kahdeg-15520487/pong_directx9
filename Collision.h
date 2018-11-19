#pragma once
#include "Utility.h"
#include "Box.h"

class Collision
{
public:
	Collision(Box* first, Box* second, Direction colDir) {
		First = first;
		Second = second;
		CollideDirection = colDir;
	}
	~Collision() {}

	Box* First;
	Box* Second;
	// direction from first box to second box
	Direction CollideDirection;
};