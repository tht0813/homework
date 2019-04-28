#include "Weather.h"
void Weather::Print()
{
	cout << "날씨는 : ";
	if (weather ==SUN)
	{
		cout << "햇빛쨍쨍" << endl;
	}
	else if (weather ==CLOUD)
	{
		cout << "흐림" << endl;
	}
	else if (weather == RAIN)
	{
		cout << "비옴" << endl;
	}
	else if (weather ==SNOW)
	{
		cout << "눈옴" << endl;
	}
	else if (weather == FOG)
	{
		cout << "안개낌" << endl;
	}

	cout << "미세먼지 수치 : ";
	if (fineDust ==VERYBAD)
	{
		cout << "매우 나쁨" << endl;
	}
	else if (fineDust == BAD)
	{
		cout << "나쁨" << endl;
	}
	else if (fineDust ==SOSO)
	{
		cout << "보통" << endl;
	}
	else if (fineDust ==GOOD)
	{
		cout << "좋음" << endl;
	}
	else if (fineDust == FANTASTIC)
	{
		cout << "매우 좋음" << endl;
	}

	//이진솔 추가 - 온도,습도

	cout << "온도 : ";
	switch (Temperature)
	{

	case VERYHOT: cout << "매우 더움" << endl; break;
	case HOT: cout << "더움" << endl; break;
	case WARM: cout << "따듯함" << endl; break;
	case COLD: cout << "추움" << endl; break;
	case VERYCOLD: cout << "매우 추움" << endl; break;

	default:
		break;
	}

	cout << "온도 : ";
	switch (Humidity)
	{

	case VERYHUMID: cout << "매우 습함" << endl; break;
	case HUMID: cout << "습함" << endl; break;
	case NORMAL: cout << "적당함" << endl; break;
	case DRY: cout << "건조함" << endl; break;
	case VERYDRY: cout << "매우 건조함" << endl; break;

	default:
		break;
	}
}


