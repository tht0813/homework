#include <iostream>
#include "Card.h"

NameCard::NameCard(string _Name, string _NumBer, string _Address, string _Rank)
{
	M_Card = new NameCard; //동적할당
	Name = _Name;
	NumBer = _NumBer;
	Address = _Address;
	Rank = _Rank;
}



void NameCard::Info()
{
	using namespace std;
	cout <<"이름 : "<<Name << endl;
	cout <<"번호 : "<<NumBer<< endl;
	cout <<"주소 : "<<Address<<endl;
	cout <<"직급 : "<<Rank<<endl;
}



//이진솔 추가 - 초기 생성에 만든 내용 수정하는 함수

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