#include "stdafx.h"
#include "Player.h"


Player::Player()
{
	score = 0;
}


Player::~Player()
{
}

bool Player::NoFlyPlane() {
	for (int i = 0; i < 4;++i) {
		if (planes[i].state == Plane::FLY)
			return false;
	}
	return true;
}

int Player::GetLandNum() {
	int k = 0;
	for (int i = 0;i < 4;++i) {
		if (planes[i].state == Plane::LAND)++k;
	}
	return k;
}