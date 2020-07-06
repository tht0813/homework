// Merchant.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <cassert>

#define SAFE_DELETE(x)      { if((x)!= nullptr) { delete (x); (x) = nullptr; } }
#define SAFE_DELETE_ARR(x)  { if((x)!= nullptr) { delete[] (x); (x) = nullptr; } }

#define EXPECT_EQ(x,y)      { assert((x)==(y) && "not same"); }

class Merchant
{
public:

	enum class eMerchantType
	{
		Seller,
		Buyer,
	};

public:
	Merchant(int a_nMoney, int a_nItemCount) :
		m_nMoney(a_nMoney), m_nItemCount(a_nItemCount)
	{
		// 
	}

	virtual void Info() const
	{
		std::cout << "돈 : " << m_nMoney << std::endl;
		std::cout << "개수 : " << m_nItemCount << std::endl;
	}



	int GetCount()
	{
		return m_nItemCount;
	}

	static const char* GetMerchatName(eMerchantType a_eType)
	{
		switch (a_eType)
		{
		case Merchant::eMerchantType::Seller:
			return "셀러";
			break;
		case Merchant::eMerchantType::Buyer:
			return "바이어";
			break;
		default:
			assert(false && "arg error");
			break;
		}
	}

	virtual eMerchantType GetType() = 0;

protected:

	int m_nMoney = 0;
	int m_nItemCount = 0;
};

class Seller : public Merchant
{
public:

	Seller(int a_nMoney, int a_nItemCount, int a_nItemPrice) :
		Merchant(a_nMoney, a_nItemCount),
		m_nItemPrice(a_nItemPrice)
	{
	}

	virtual void Info() const override
	{
		Merchant::Info();
		std::cout << "가격 : " << m_nItemPrice << std::endl;
	}

	int GetItemPrice()
	{
		return m_nItemPrice;
	}

	void Buy(int a_nCount)
	{
		assert(a_nCount > 0 && "arg error");

		m_nItemCount -= a_nCount;
		m_nMoney += (m_nItemPrice * a_nCount);

		// 잘 팜 
	}

	// Merchant을(를) 통해 상속됨
	virtual eMerchantType GetType() override
	{
		return eMerchantType::Seller;
	}

private:

	int m_nItemPrice;
};

class Buyer : public Merchant
{
public:

	Buyer(int a_nMoney, int a_nItemCount) : Merchant(a_nMoney, a_nItemCount) { }

	bool Buy(int a_nCount, Seller* a_pSeller)
	{
		if (a_nCount <= 0 || a_pSeller == nullptr) {
			//cout << "arg error" << ed
			return false;
		}

		int nCount = a_pSeller->GetCount();

		if (a_nCount > nCount) { /*cout << "셀러가 몇개없으" << ed*/return false; }

		int nPrice = a_pSeller->GetItemPrice();
		nPrice *= nCount;

		if (m_nMoney < nPrice) { return false; }

		a_pSeller->Buy(a_nCount);

		m_nItemCount += a_nCount;
		m_nMoney -= nPrice;
	}

	// Merchant을(를) 통해 상속됨
	virtual eMerchantType GetType() override
	{
		return eMerchantType::Buyer;
	}
};



int main(int n, char* s[])
{
	using namespace std;

	Merchant * p1 = new Buyer(1000, 10);
	Merchant * p2 = new Seller(1000, 1000, 10);

	p1->Info();
	p2->Info();

	p1.Buy(5, &p2);


	cout << Merchant::GetMerchatName(p1->GetType()) << endl;
	cout << Merchant::GetMerchatName(p2->GetType()) << endl;

	SAFE_DELETE(p1);
	SAFE_DELETE(p2);

	getchar();
	return 0;
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
