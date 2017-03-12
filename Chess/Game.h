#pragma once
#include"Player.h"
#include<ctime>
#include<cstdlib>
class Game
{
	//void romRurn();
public:
	Game();
	~Game();

	void Go(int player_id, int plane_id, int step);
	void Start();//¿ªÊ¼ÓÎÏ·
	void NextTurn();
	Player& GetPlayer(int player_id);

	int turn;

private:
	Player players[4];
};

