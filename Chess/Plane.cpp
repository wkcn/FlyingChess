#include "stdafx.h"
#include "Plane.h"


Plane::Plane()
{
	steps = 0;
	state = NOFLY;
	first = true;
	animation = false;
}


Plane::~Plane()
{
}
	
void Plane::Go(int s) {
	//history.push(GetWorldPosI(steps));
	steps += s;
	if (steps > 56) {
		history.push(GetWorldPosI(56));
		steps = 56 - (steps - 56);
	}else if (steps == 56)state = LAND;
	else if (steps == 18) {
		history.push(GetWorldPosI(steps));
		steps = 30;
	}else if (steps % 4 == 2 && steps < 50){
		history.push(GetWorldPosI(steps));
		steps += 4;
	}
	if (steps == 56)state = LAND;
	animation = true;
}

int Plane::GetWorldPos() {
	return GetWorldPosI(steps);
}

int Plane::GetWorldPosI(int steps) {
	if (steps > 50)return (steps - 50) + 52 + color * 6;
	{
		int k = (steps + 13 * color) % 52;
		if (k == 0)return 52;
		return k;
	}
}

void Plane::Crash() {
	steps = 0;
	state = NOFLY;
	first = true;
}