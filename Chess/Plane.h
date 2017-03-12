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
	int color;//0��1��2��3������ɫ(��Ӧ4����ҵķɻ�)
	int steps;//�����ߵĲ���
	PLANE_STATE state;//0��1��2��3��4����״̬��û����ɣ��պ���ɣ��Ѿ���ɣ���ʼ���䣬�Ѿ����䣩
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

