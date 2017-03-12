#pragma once
#include <queue>
using namespace std;

class Plane
{
public:
enum PLANE_STATE {
	NOFLY, FLY, LAND
};
public:
	int id;
	int color;//0、1、2、3四种颜色(对应4个玩家的飞机)
	int steps;//棋子走的步数
	PLANE_STATE state;//0、1、2、3，4五种状态（没有起飞，刚好起飞，已经起飞，开始降落，已经降落）
	queue<int> history;
	double x, y;
	bool first;
	bool animation;
public:
	void Go(int s);
	int GetWorldPos();
	int GetWorldPosI(int);
	void Crash();
	Plane();
	~Plane();
};

