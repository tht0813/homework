#pragma once


#include <string>
using namespace std;


class NameCard {
public:
	NameCard(){}
	NameCard(string _Name, string _NumBer, string _Address, string _Rank);
	~NameCard(){
		delete M_Card; //�Ҵ�����
		M_Card = nullptr; 
	}
	void Info(); //�������


	bool ChangeNumber(string s_Number); //��ȭ��ȣ ����
	bool ChangeAddress( string s_Address); //�ּҼ���
	bool ChangeRank(string s_Rank); //���޼���
	

private:
	NameCard* M_Card = nullptr;
	string Name; //�̸�
	string NumBer; //��ȭ��ȣ
	string Address; //ȸ���ּ�
	string Rank; //����
};