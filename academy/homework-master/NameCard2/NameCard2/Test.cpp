#include "Card.h"
#include <iostream>

int main()
{
	NameCard MyCard("ȫ�浿", "011123", "����", "�븮");
	MyCard.Info();
	std::cout << std::endl;

	MyCard.ChangeAddress("���");
	MyCard.ChangeNumber("0123456");
	MyCard.ChangeRank("����");

	MyCard.Info();
	getchar();
	return 0;
}