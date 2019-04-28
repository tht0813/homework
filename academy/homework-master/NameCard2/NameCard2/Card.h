#pragma once


#include <string>
using namespace std;


class NameCard {
public:
	NameCard(){}
	NameCard(string _Name, string _NumBer, string _Address, string _Rank);
	~NameCard(){
		delete M_Card; //할당해제
		M_Card = nullptr; 
	}
	void Info(); //정보출력


	bool ChangeNumber(string s_Number); //전화번호 수정
	bool ChangeAddress( string s_Address); //주소수정
	bool ChangeRank(string s_Rank); //직급수정
	

private:
	NameCard* M_Card = nullptr;
	string Name; //이름
	string NumBer; //전화번호
	string Address; //회사주소
	string Rank; //직급
};