#include "Weather.h"
void Weather::Print()
{
	cout << "������ : ";
	if (weather ==SUN)
	{
		cout << "�޺�¸¸" << endl;
	}
	else if (weather ==CLOUD)
	{
		cout << "�帲" << endl;
	}
	else if (weather == RAIN)
	{
		cout << "���" << endl;
	}
	else if (weather ==SNOW)
	{
		cout << "����" << endl;
	}
	else if (weather == FOG)
	{
		cout << "�Ȱ���" << endl;
	}

	cout << "�̼����� ��ġ : ";
	if (fineDust ==VERYBAD)
	{
		cout << "�ſ� ����" << endl;
	}
	else if (fineDust == BAD)
	{
		cout << "����" << endl;
	}
	else if (fineDust ==SOSO)
	{
		cout << "����" << endl;
	}
	else if (fineDust ==GOOD)
	{
		cout << "����" << endl;
	}
	else if (fineDust == FANTASTIC)
	{
		cout << "�ſ� ����" << endl;
	}

	//������ �߰� - �µ�,����

	cout << "�µ� : ";
	switch (Temperature)
	{

	case VERYHOT: cout << "�ſ� ����" << endl; break;
	case HOT: cout << "����" << endl; break;
	case WARM: cout << "������" << endl; break;
	case COLD: cout << "�߿�" << endl; break;
	case VERYCOLD: cout << "�ſ� �߿�" << endl; break;

	default:
		break;
	}

	cout << "�µ� : ";
	switch (Humidity)
	{

	case VERYHUMID: cout << "�ſ� ����" << endl; break;
	case HUMID: cout << "����" << endl; break;
	case NORMAL: cout << "������" << endl; break;
	case DRY: cout << "������" << endl; break;
	case VERYDRY: cout << "�ſ� ������" << endl; break;

	default:
		break;
	}
}


