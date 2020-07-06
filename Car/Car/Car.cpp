// Car.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

#define SAFE_DELETE(x)      { if((x)!= nullptr) { delete (x); (x) = nullptr; } }
#define SAFE_DELETE_ARR(x)  { if((x)!= nullptr) { delete[] (x); (x) = nullptr; } }

using namespace std;

class CarGame
{
public:
	CarGame(int n_fuel) : C_fuel(n_fuel)
	{
	}
	virtual void info() const
	{
		cout << "잔여 연료 :" << C_fuel << endl;
	}

protected:
	int C_fuel = 0;
};

class Car : public CarGame
{
public:
	int money;
	int km_h;
	Car(int cmoney, int ckm_h, int cfuel) : money(cmoney), km_h(ckm_h), CarGame(cfuel)
	{
	}
	virtual void info() const override
	{
		int y_fuel = (km_h/10);
		cout << "잔여 금액 : " << money << endl;
		cout << "시속 : " << km_h << endl;
		cout << "최대 이동량 : " << y_fuel * C_fuel << endl;
		cout << "연비 : " << y_fuel << endl;
		CarGame::info();
	}
	int GetFuel() { return C_fuel; }
	int GetMoney() { return money; }
	void Buy(int a_gfuel, int a_gPrice)
	{
		C_fuel += a_gfuel;
		money -= (a_gfuel*a_gPrice);
	}
};

class GasStation : public CarGame
{
public:
	int lPrice;


	GasStation(int nlPrice, int nlFuel) : lPrice(nlPrice), CarGame(nlFuel)
	{
	}
	virtual void info() const override
	{
		cout << "리터당 가격 : " << lPrice << endl;
		CarGame::info();
	}
	bool Buy(int a_gfuel, Car* a_gCar)
	{
		if (a_gfuel <= 0 || a_gCar == nullptr) { cout << "arg error" << endl; return false; }
		
		int gfuel = a_gCar->GetFuel();
		if (gfuel >= 100) { cout << "연료가 가득 찼습니다.\n" << endl; return false; }
		
		int gPrice = a_gCar->GetMoney();
		if (gPrice < (lPrice*a_gfuel)) { cout << "잔액이 부족합니다.\n" << endl; return false; }

		C_fuel -= a_gfuel;
		a_gCar->Buy(a_gfuel, lPrice);
		cout << "구매 완료\n" << endl;
	}
	int GetgPrice() { return lPrice; }
};


int main()
{
	CarGame* sonata = new Car(100000, 80, 50);
	cout << "소나타" << endl;
	sonata->info();

	CarGame* goyang = new GasStation(1550, 50);
	cout << "고양주유소" << endl;
	goyang->info();

	goyang.Buy(20, &sonata);
	cout << "소나타" << endl;
	sonata->info();

	cout << "고양주유소" << endl;
	goyang->info();


}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
