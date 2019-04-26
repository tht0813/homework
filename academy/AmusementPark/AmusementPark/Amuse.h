#pragma once
#include "Human.h"
class Amuse
{
public:
	Amuse(int a_Limit, int a_PlayTime, int a_Waiting);
	~Amuse();
	void Info();
	void WaitingTime(Human h);

private:
	int Limit=0;
	int PlayTime = 0;
	int Waiting = 0;
};

