#include "pch.h"
#include "Amuse.h"


Amuse::Amuse(int a_Limit, int a_PlayTime, int a_Waiting) :
	Limit{ a_Limit }, PlayTime{ a_PlayTime }, Waiting{ a_Waiting } { }


Amuse::~Amuse()
{
}

void Amuse::Info()
{
	cout <<"정원 : " << Limit << "명" << endl;
	cout << "운행 소요시간 : " << PlayTime << "명" << endl;
	cout << "기다리는 사람의 수 : " << Waiting << "명" << endl;
}

void Amuse::WaitingTime(Human h)
{

	int wtime;
	int pNum = h.GetPeopleNum();
	
	wtime = (Waiting) / (Limit) * (PlayTime);
	if ((Waiting) % (Limit) < pNum) { wtime+=PlayTime; }

	cout << "기다려야 하는 시간 : " << wtime << "분" << endl;
}

