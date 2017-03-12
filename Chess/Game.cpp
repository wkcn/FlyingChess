#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	srand(time(NULL));
	for (int i = 0; i < 4; ++i) {
		for (int k = 0; k < 4; ++k) {
			players[i].planes[k].color = i;
			players[i].planes[k].id = k;
			players[i].planes[k].state = Plane::NOFLY;
			players[i].planes[k].steps = 0;
		}
	}
	Start();
	/*players[0].planes[0].state = Plane::FLY;
	players[0].planes[0].steps = 1;
	players[1].planes[0].state = Plane::FLY;
	players[1].planes[0].steps = 1;
	players[2].planes[0].state = Plane::FLY;
	players[2].planes[0].steps = 1;
	players[3].planes[0].state = Plane::FLY;
	players[3].planes[0].steps = 1;*/
}

void Game::Go(int player_id, int plane_id, int step){
	players[player_id].planes[plane_id].Go(step);
	int mypos = players[player_id].planes[plane_id].GetWorldPos();
	for (int i = 0; i < 4; ++i) {
		if (i == player_id)continue;
		for (int k = 0; k < 4; ++k) {
			Plane &other = players[i].planes[k];
			int tpos = other.GetWorldPos();
			if (mypos == tpos) {
				other.Crash();
			}
			else {
				queue<int> q = players[player_id].planes[plane_id].history;
				while (!q.empty()) {
					int w = q.front();q.pop();
					if (w == tpos) {
						other.Crash();
						break;
					}
				}
			}
		}
	}
}

void Game::Start() {
	turn = rand() % 4;
}


Game::~Game()
{
}

Player& Game::GetPlayer(int player_id) {
	return players[player_id];
}

void Game::NextTurn() {
	turn = (turn + 1) % 4;
}