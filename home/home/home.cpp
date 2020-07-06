// home.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <pch.h>
#include <iostream>
#include <Turboc.h>



#define LEFT 75

#define RIGHT 77

#define UP 72

#define DOWN 80

#define MAXSTAGE 3

#define putchxy(x,y,c) {gotoxy(x,y);putch(c);}



void DrawScreen();

bool TestEnd();

void Move(int dir);



char ns[18][21];

int nStage;

int nx, ny;

int nMove;



char arStage[MAXSTAGE][18][21] = {

	 {

	 "####################",

	 "####################",

	 "####################",

	 "#####   ############",

	 "#####O  ############",

	 "#####  O############",

	 "###  O O ###########",

	 "### # ## ###########",

	 "#   # ## #####  ..##",

	 "# O  O   @      ..##",

	 "##### ### # ##  ..##",

	 "#####     ##########",

	 "####################",

	 "####################",

	 "####################",

	 "####################",

	 "####################",

	 "####################"

	 },

	 {

	 "####################",

	 "####################",

	 "####################",

	 "####################",

	 "####..  #     ######",

	 "####..  # O  O  ####",

	 "####..  #O####  ####",

	 "####..    @ ##  ####",

	 "####..  # #  O #####",

	 "######### ##O O ####",

	 "###### O  O O O ####",

	 "######    #     ####",

	 "####################",

	 "####################",

	 "####################",

	 "####################",

	 "####################",

	 "####################"

	 },

	 {

	 "####################",

	 "####################",

	 "####################",

	 "####################",

	 "##########     @####",

	 "########## O#O #####",

	 "########## O  O#####",

	 "###########O O #####",

	 "########## O # #####",

	 "##....  ## O  O  ###",

	 "###...    O  O   ###",

	 "##....  ############",

	 "####################",

	 "####################",

	 "####################",

	 "####################",

	 "####################",

	 "####################"

	 },

};



void main()

{

	int ch;

	int x, y;



	setcursortype(NOCURSOR);

	nStage = 0;



	for (; 1;) {

		memcpy(ns, arStage[nStage], sizeof(ns));

		for (y = 0; y < 18; y++) {

			for (x = 0; x < 20; x++) {

				if (ns[y][x] == '@') {

					nx = x;

					ny = y;

					ns[y][x] = ' ';

				}

			}

		}

		clrscr();

		nMove = 0;



		for (; 2;) {

			DrawScreen();

			ch = getch();

			if (ch == 0xE0 || ch == 0) {

				ch = getch();

				switch (ch) {

				case LEFT:

				case RIGHT:

				case UP:

				case DOWN:

					Move(ch);

					break;

				}

			}
			else {

				ch = tolower(ch);

				if (ch == 'r') {

					break;

				}

				if (ch == 'n') {

					if (nStage < MAXSTAGE - 1) {

						nStage++;

					}

					break;

				}

				if (ch == 'p') {

					if (nStage > 0) {

						nStage--;

					}

					break;

				}

				if (ch == 'q') {

					setcursortype(NORMALCURSOR);

					exit(0);

				}

			}



			if (TestEnd()) {

				clrscr();

				gotoxy(10, 10);

				printf("%d 스테이지를 풀었습니다. 다음 스테이지로 이동합니다",

					nStage + 1);

				delay(2000);

				if (nStage < MAXSTAGE - 1) {

					nStage++;

				}

				break;

			}

		}

	}

}



void DrawScreen()

{

	int x, y;

	for (y = 0; y < 18; y++) {

		for (x = 0; x < 20; x++) {

			putchxy(x, y, ns[y][x]);

		}

	}

	putchxy(nx, ny, '@');



	gotoxy(40, 2); puts("SOKOBAN");

	gotoxy(40, 4); puts("Q:종료, R:다시 시작");

	gotoxy(40, 6); puts("N:다음, P:이전");

	gotoxy(40, 8); printf("스테이지 : %d", nStage + 1);

	gotoxy(40, 10); printf("이동 회수 : %d", nMove);

}



BOOL TestEnd()

{

	int x, y;



	for (y = 0; y < 18; y++) {

		for (x = 0; x < 20; x++) {

			if (arStage[nStage][y][x] == '.' && ns[y][x] != 'O') {

				return FALSE;

			}

		}

	}

	return TRUE;

}



void Move(int dir)

{

	int dx = 0, dy = 0;



	switch (dir) {

	case LEFT:

		dx = -1;

		break;

	case RIGHT:

		dx = 1;

		break;

	case UP:

		dy = -1;

		break;

	case DOWN:

		dy = 1;

		break;

	}



	if (ns[ny + dy][nx + dx] != '#') {

		if (ns[ny + dy][nx + dx] == 'O') {

			if (ns[ny + dy * 2][nx + dx * 2] == ' ' || ns[ny + dy * 2][nx + dx * 2] == '.') {

				if (arStage[nStage][ny + dy][nx + dx] == '.') {

					ns[ny + dy][nx + dx] = '.';

				}
				else {

					ns[ny + dy][nx + dx] = ' ';

				}

				ns[ny + dy * 2][nx + dx * 2] = 'O';

			}
			else {

				return;

			}

		}

		nx += dx;

		ny += dy;

		nMove++;

	}

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
