#include "pch.h"
#include "Amuse.h"


Amuse::Amuse(int a_Limit, int a_PlayTime, int a_Waiting) :
	Limit{ a_Limit }, PlayTime{ a_PlayTime }, Waiting{ a_Waiting } { }


Amuse::~Amuse()
{
}

void Amuse::Info()
{
	cout <<"���� : " << Limit << "��" << endl;
	cout << "���� �ҿ�ð� : " << PlayTime << "��" << endl;
	cout << "��ٸ��� ����� �� : " << Waiting << "��" << endl;
}

void Amuse::WaitingTime(Human h)
{

	int wtime;
	int pNum = h.GetPeopleNum();
	
	wtime = (Waiting) / (Limit) * (PlayTime);
	if ((Waiting) % (Limit) < pNum) { wtime+=PlayTime; }

	cout << "��ٷ��� �ϴ� �ð� : " << wtime << "��" << endl;
}

