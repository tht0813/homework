#include "Card.h"
#include <iostream>

int main()
{
	NameCard MyCard("홍길동", "011123", "서울", "대리");
	MyCard.Info();
	std::cout << std::endl;

	MyCard.ChangeAddress("고양");
	MyCard.ChangeNumber("0123456");
	MyCard.ChangeRank("사장");

	MyCard.Info();
	getchar();
	return 0;
}