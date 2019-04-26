#include "pch.h"
#include "Human.h"


Human::Human(int h_PeopleNum) : PeopleNum{h_PeopleNum} { }
Human::~Human() { }

int Human::GetPeopleNum()
{
	return PeopleNum;
}
