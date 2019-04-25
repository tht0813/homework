#pragma once
#include "GameProgram.h"

class Player
{
public:
	Player() = default;
	~Player();
	int Input();
	void GameStart();

private:
	int Record[2]{ 0, };
	GameProgram Game;
	int count = 3;
};

