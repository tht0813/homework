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
	cout << count << "�� ���ҽ��ϴ�." << "������ �Է��ϼ���. ��� : 0 ������ : 1 ���� : 2" << endl;
	int pInput = Input();
	int gReturn = Game.Return();
	
	if (pInput < 0 || pInput>2)
	{
	while (pInput<0 || pInput>2)
	{
		cout << "�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ���." << endl;
		pInput = Input();
	}
	}
	
	if (pInput == gReturn)
	{
		cout << "�ɷȽ��ϴ�."<<endl;
		cout<<"���� ����� : 3" << endl;
		cout << "���� ���� : 4" << endl;
		while (true)
		{
			pInput = Input();
			if (pInput == 3)
			{
				count = 3;
				system("cls");
				GameStart();
			}
			else if (pInput == 4) { return; }
			else
			{
				cout << "�ٽ� �Է��ϼ���." << endl;
			}
		}
	}
	else
	{
		cout << "���߽��ϴ�." << endl;
		count--;
		if (count == 0) 
		{ 
			cout << "�¸�!! �ٽ� �����Ͻðڽ��ϱ�? ( �� : 3  �ƴϿ� : 4 )" << endl; 
			while (true)
			{
				pInput = Input();
				if (pInput == 3) { count = 3; system("cls"); GameStart(); }
				else if (pInput == 4) { return; }
				else { cout<<"�ٽ� �Է��ϼ���."<<endl; }
			}
		}

		GameStart();
	}
}
