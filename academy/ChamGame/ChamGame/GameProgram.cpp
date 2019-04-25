#include "pch.h"
#include "GameProgram.h"


GameProgram::GameProgram()
{
}
GameProgram::~GameProgram()
{
}

int GameProgram::Return()
{
	srand(time(0));

	return rand()%3;
}
