#pragma once

//#include "CHess.h"
#include "Plane.h"

class Player
{
public:
	Plane planes[4];
	int score;

	//����
	//bool out();//��ʾ�Ƿ��Ѿ��зɻ���ȥ��
	//void run();//
	int GetLandNum();
	bool NoFlyPlane();

public:
	Player();
	~Player();
};

