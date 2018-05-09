#include "Tetris_Header.h"

extern int stage[22][12];
extern int holdBlock;
extern int holdMotion;
extern int score;
extern int nextBlock[2];
extern int leveltime;
extern int clientCount;
extern int mode;
extern char menus[4][30];
extern char playerName[3][50];

void printGameOver() {
	setColor(WHITE);
	gotoxy(16, 2); printf("┏━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(16, 3); printf("┃      GAME OVER      ┃ ");
	gotoxy(16, 4); printf("┗━━━━━━━━━━━━━━━━━━━━━┛");

	gotoxy(17, 14); printf("━━  Press Enter Key ━━");

}
void printGameWin() {
	setColor(WHITE);
	gotoxy(16, 2); printf("┏━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(16, 3); printf("┃      W   I   N      ┃ ");
	gotoxy(16, 4); printf("┗━━━━━━━━━━━━━━━━━━━━━┛");

	gotoxy(17, 14); printf("━━  Press Enter Key ━━");

}
void printGameStart() {
	setColor(WHITE);
	gotoxy(16, 2); printf("┏━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(16, 3); printf("┃      GAME START     ┃ ");
	gotoxy(16, 4); printf("┗━━━━━━━━━━━━━━━━━━━━━┛");

	gotoxy(17, 14); printf("━━  Press Enter Key ━━");

}
void printGamePause() {
	setColor(WHITE);
	gotoxy(16, 2); printf("┏━━━━━━━━━━━━━━━━━━━━━┓");
	gotoxy(16, 3); printf("┃      GAME PAUSE     ┃ ");
	gotoxy(16, 4); printf("┗━━━━━━━━━━━━━━━━━━━━━┛");

	gotoxy(18, 14); printf("━━  Press P Key  ━━");
}
void printNextBlock() {
	for (int i = 0; i < 4; i++) {
		putsxy(44, 2 + i, "        ");
		putsxy(44, 9 + i, "        ");
	}
	int x1 = nextBlock[0] == 5 ? 46 : 45;
	int y1 = nextBlock[0] == 6 ? 2 : 3;
	int x2 = nextBlock[1] == 5 ? 46 : 45;
	int y2 = nextBlock[1] == 6 ? 9 : 10;

	createBlock(x1, y1, nextBlock[0], 0);
	createBlock(x2, y2, nextBlock[1], 0);
}
void printHoldBlock() {
	for (int i = 0; i < 4; i++)
		putsxy(4, 2 + i, "        ");

	if (holdBlock != -1) {
		int x = holdBlock == 5 ? 6 : 5;
		int y = holdBlock == 6 ? 2 : 3;
		if (holdBlock == 6 && (holdMotion == 1 || holdMotion == 3)) x = 4;
		createBlock(x, y, holdBlock, holdMotion);
	}
}
void printStage() {
	int i, j;
	for (i = 0; i < 22; i++) {
		gotoxy(16, i);
		for (j = 0; j < 12; j++) {
			selectColor(stage[i][j]);
			if (stage[i][j] == 7) { setColor(WHITE); printf("▣"); }
			else if (stage[i][j] == 8) printf("  ");
			else printf("■");
		}
	}
}
void printOtherPlayer() {
	int initPos_x = 29;
	int i, j;
	for (int playerCNT = 0; playerCNT < clientCount; playerCNT++) {

		initPos_x += 27;

		for (i = 0; i < 22; i++) {// 칸막이그리기
			gotoxy(initPos_x, i); printf("|");
		}


		for (i = 0; i < 22; i++) {
			gotoxy(initPos_x + 2, i);
			for (j = 0; j < 12; j++) {
				selectColor(stage[i][j]);
				if (stage[i][j] == 7) { setColor(WHITE); printf("▣"); }
				else if (stage[i][j] == 8) printf("  ");
				else printf("■");
			}
		}

		//		if (strcmp(myName, playerName[playerCNT]) != 0) {
		putsxy(70 - (int)(strlen(playerName[playerCNT]) / 2) + (27 * playerCNT)
			, 22
			, playerName[playerCNT]);
		//		}

	}
}
void printBlockSpace() {
	int i;
	setColor(WHITE);
	char outline[8][17] = {
		"□□□□□□",
		"□        □",
		"□        □",
		"□        □",
		"□        □",
		"□□□□□□"
	};
	putsxy(43, 0, "Next Block");
	putsxy(3, 0, "Hold Block");

	for (i = 0; i < 8; i++) {
		putsxy(2, 1 + i, outline[i]);
		putsxy(42, 1 + i, outline[i]);
		putsxy(42, 8 + i, outline[i]);
	}

}
void printInfo() {
	int i;
	setColor(WHITE);
	for (i = 0; i < 22; i++) {
		gotoxy(56, i); printf("|");
	}
	gotoxy(2, 8); printf("스테이지 : %.2d", 11 - leveltime / 100);
	gotoxy(2, 10); printf("점수 : %d", score);

	if (ISSINGLE) {
		gotoxy(59, 1); printf("┏━━━━━━━━━━━━━━┓");
		gotoxy(59, 2); printf("┃  T E T R I S ┃ ");
		gotoxy(59, 3); printf("┗━━━━━━━━━━━━━━┛");

		gotoxy(66, 5); printf("회전");
		gotoxy(67, 6); printf("↑");
		gotoxy(60, 7); printf("LEFT ←  → RIGHT");
		gotoxy(67, 8); printf("↓");
		gotoxy(66, 9); printf("DOWN");
		gotoxy(60, 11); printf("Space Bar:퀵다운");
		gotoxy(60, 13); printf("Z Key:블록 홀드");
		gotoxy(60, 15); printf("C Key:블록 역회전");
		gotoxy(60, 17); printf("P Key:일시정지");
		gotoxy(60, 19); printf("R Key:게임 재시작");

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

// 메인페이지를 보여준다.
void printMainPage() {

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

