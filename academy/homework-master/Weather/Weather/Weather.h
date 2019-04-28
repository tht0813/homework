#pragma once
#include <iostream>
#include <Windows.h>
#include <time.h>
#include <cstdlib>
using namespace std;





class Weather {
public:
	Weather() { 
		/*M_Weather = new Weather;*/
		srand(time(nullptr));
		weather = rand() % 5;
		fineDust = rand() % 5; 
		Temperature = rand() % 5;
		Humidity = rand() % 5;
	}
	~Weather() {
		/*delete M_Weather;
		M_Weather = nullptr;*/
	}

	void Print();


private:
	int weather;
	int fineDust;
	int Temperature;
	int Humidity;
	Weather* M_Weather = nullptr;

enum DUST
{
	VERYBAD,
	BAD,
	SOSO,
	GOOD,
	FANTASTIC,
};

enum  WEATHER
{
	SUN,
	CLOUD,
	RAIN,
	SNOW,
	FOG,
};

//이진솔 추가 - 온도,습도
enum TEMPERATURE
{
	VERYHOT,
	HOT,
	WARM,
	COLD,
	VERYCOLD,
};

enum HUMIDITY
{
	VERYHUMID,
	HUMID,
	NORMAL,
	DRY,
	VERYDRY,
};

};