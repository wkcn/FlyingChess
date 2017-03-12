#pragma once

//#include "CHess.h"
#include "Plane.h"

class Player
{
public:
	Plane planes[4];
	int score;

	//函数
	//bool out();//表示是否已经有飞机出去了
	//void run();//
	int GetLandNum();
	bool NoFlyPlane();

public:
	Player();
	~Player();
};

