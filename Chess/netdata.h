#ifndef _NETDATA_H_
#define _NETDATA_H_

#include "game.h"

struct NetData{
	char CHECK_CODE[16];
	int turn;
	int dice;
	int dice_p;
	int steps[4][4];
	int select_mode;
	Plane::PLANE_STATE states[4][4];
};

void GetNetData(NetData &data, Game &game);
bool ApplyNetData(NetData &data, Game &game);

#endif