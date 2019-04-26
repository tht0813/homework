#include "pch.h"
#include "Player.h"




Player::~Player()
{
}

int Player::Input()
{
	int input;
	cin >> input;
	return input;
}

void Player::GameStart()
{
	cout << count << "번 남았습니다." << "방향을 입력하세요. 가운데 : 0 오른쪽 : 1 왼쪽 : 2" << endl;
	int pInput = Input();
	int gReturn = Game.Return();
	

	if (pInput == gReturn)
	{
		cout << "걸렸습니다."<<endl;
		cout<<"게임 재시작 : 3" << endl;
		cout << "게임 종료 : 4" << endl;
		
		pInput = Input();
		if (pInput == 3)
		{
			GameStart();
		}
		else if (pInput == 4) {  return; }
		else
		{ 
			cout << "다시 입력하세요." << endl; 
		}
	}
	else
	{
		cout << "피했습니다." << endl;
		count--;
		if (count == 0) 
		{ 
			cout << "승리!! 다시 시작하시겠습니까? ( 예 : 3  아니오 : 4 )" << endl; 
			pInput = Input();
			if (pInput == 3) { count = 3; GameStart(); }
			else { return; }
		}

		GameStart();
	}
}
