#include <iostream>
#include "Card.h"

NameCard::NameCard(string _Name, string _NumBer, string _Address, string _Rank)
{
	M_Card = new NameCard; //�����Ҵ�
	Name = _Name;
	NumBer = _NumBer;
	Address = _Address;
	Rank = _Rank;
}



void NameCard::Info()
{
	using namespace std;
	cout <<"�̸� : "<<Name << endl;
	cout <<"��ȣ : "<<NumBer<< endl;
	cout <<"�ּ� : "<<Address<<endl;
	cout <<"���� : "<<Rank<<endl;
}



//������ �߰� - �ʱ� ������ ���� ���� �����ϴ� �Լ�

bool NameCard::ChangeNumber(string s_Number)
{
	NumBer = s_Number;
	return true;
}

bool NameCard::ChangeAddress(string s_Address)
{
	Address = s_Address;
	return true;
}

bool NameCard::ChangeRank(string s_Rank)
{
	Rank = s_Rank;
	return true;
}