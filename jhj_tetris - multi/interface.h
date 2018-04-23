#pragma once
#include "tools.h"

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

