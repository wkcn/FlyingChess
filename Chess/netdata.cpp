#include "stdafx.h"
#include "netdata.h"
#include <iostream>
#include <ctime>
using namespace std;

const char CHECK_CODE[16] = "abcdflightabcde";

void GetNetData(NetData &data, Game &game) {
	cout << "Send Net" << endl;
	memcpy(data.CHECK_CODE, CHECK_CODE, 16);
	for (int i = 0;i < 4;++i) {
		Player &p = game.GetPlayer(i);
		for (int j = 0;j < 4;++j) {
			Plane &a = p.planes[j];
			data.steps[i][j] = a.steps;
			data.states[i][j] = a.state;
		}
	}
	data.turn = game.turn;
}

bool ApplyNetData(NetData &data, Game &game) {
	cout << "Apply" << endl;
	//Check
	for (int i = 0;i < 16;++i) {
		if (data.CHECK_CODE[i] != CHECK_CODE[i])return false;
	}
	cout << time(0) << "Apply Check OK" << endl;
	for (int i = 0;i < 4;++i) {
		Player &p = game.GetPlayer(i);
		for (int j = 0;j < 4;++j) {
			Plane &a = p.planes[j];
			if (data.states[i][j] != Plane::NOFLY && (a.steps != data.steps[i][j] || a.state != data.states[i][j]))a.animation = true;
			a.steps = data.steps[i][j];
			a.state = data.states[i][j];
		}
	}

	game.turn = data.turn;
	return true;
}