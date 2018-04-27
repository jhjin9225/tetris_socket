#include "playingGame.h"
#include "basic.h"
#include "blockes.h"
#include "initalize.h"
#include "interface.h"
#include "pages.h"
#include "queue.h"
#include "thread.h"

void initMain();
void printTETRIS();
void showMainPage();

char menus[4][30] = {
	{ "S I N G L E  P L A Y" },
{ "M U L T I  P L A Y" },
{ "S E T T I N G" },
{ "E X I T" }
};


int main(void) {
	
	initMain();
	showMainPage();
	
	return 0;
}

// 메인페이지를 보여준다.
void showMainPage() {

	printTETRIS();	// 메인화면에 TETRIS 글자출력

	int menu_X = 26;	// 메뉴선택창의 x 위치
	int menu_Y = 12;	// 메뉴선택창의 y 위치
	int selectedMenu = 0;

	// 메뉴 선택(글자 색깔 변경)
	for (int i = 0; i < 3; i++) {
		if (selectedMenu == i)
			setColor(GREEN);
		else
			setColor(WHITE);
		putsxy(menu_X, menu_Y + (i * 2), menus[i]);
	}

	int flag = 1;
	while (flag) {	// 메뉴 선택

		char ch = _getch();	//키보드값 1개를 받아 메뉴 선택
		switch (ch) {
		case DOWN:
			if (selectedMenu >= 3)	selectedMenu = 3;	// 마지막 메뉴 제한
			else {	// 현재 메뉴를 흰글자로 바꾸고 다음선택한 메뉴 글자를 초록색으로 변경
				setColor(WHITE);	putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
				selectedMenu++;
				setColor(GREEN);	putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
			}
			break;
		case UP:
			if (selectedMenu <= 0)	selectedMenu = 0;	// 첫 메뉴 제한
			else {	// 현재 메뉴를 흰글자로 바꾸고 다음선택한 메뉴 글자를 초록색으로 변경
				setColor(WHITE);	putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
				selectedMenu--;
				setColor(GREEN);	putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
			}
			break;
		case ENTER:
			if (selectedMenu == 0) { mode = SINGLEMODE; gameStart(); }	// 싱글모드 실행
			else if (selectedMenu == 1) { multiSetting(); }	// 멀티모드 설정 실행
			else if (selectedMenu == 2) { configPage(); }	// 설정모드 실행
			else if (selectedMenu == 3) exit(1);	// 종료

			printTETRIS();	// 메인모드 초기화면
			setColor(GREEN); putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
			break;
		}
	}
}

// 'TETRIS'글자출력
void printTETRIS() {
	clrscr();
	int tetris[6][5][3] = {
		{ { 1,1,1 },
	{ 0,1,0 },
	{ 0,1,0 },
	{ 0,1,0 },
	{ 0,1,0 }
		},
	{ { 1,1,1 },
	{ 1,0,0 },
	{ 1,1,1 },
	{ 1,0,0 },
	{ 1,1,1 }
	},
{
	{ 1,1,1 },
	{ 0,1,0 },
	{ 0,1,0 },
	{ 0,1,0 },
	{ 0,1,0 }
},
{
	{ 1,1,0 },
	{ 1,0,1 },
	{ 1,1,1 },
	{ 1,0,1 },
	{ 1,0,1 }
},
{
	{ 1,1,1 },
	{ 0,1,0 },
	{ 0,1,0 },
	{ 0,1,0 },
	{ 1,1,1 }
},
{
	{ 1,1,1 },
	{ 1,0,0 },
	{ 1,1,1 },
	{ 0,0,1 },
	{ 1,1,1 }
}
	};

	int i, j, k;
	setColor(WHITE);
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 5; j++) {
			gotoxy(16 + i * 8, 4 + j);
			for (k = 0; k < 3; k++) {
				selectColor(i);
				if (tetris[i][j][k] == 1) printf("■");
				else printf("  ");
			}
		}
	}
	setColor(WHITE);
	putsxy(26, 12, menus[0]);
	putsxy(26, 14, menus[1]);
	putsxy(26, 16, menus[2]);
	putsxy(26, 18, menus[3]);
	setColor(WHITE);
	putsxy(55, 21, "made by SOMA, JHJ");
}

void initMain() {
	srand((unsigned)time(NULL)); // 시간으로 랜덤 설정
	setcursortype(NOCURSOR); //커서 깜박임 제거
	resz(80, 24);	//콘솔의 크기 설정
}
