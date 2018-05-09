#include "Tetris_Header.h"


extern boolean isTurnMode;
extern boolean isSpeedMode;
extern boolean isPreViewMode;

// 설정페이지를 보여주고 설정을 변경한다.
void configPage() {

	int menuCount = 0;
	int save_menuCount = -1;
	boolean isconfig = TRUE;
	boolean isTurn = TRUE;
	boolean isSpeed = FALSE;
	boolean isPreView = FALSE;

	char rowLine[] = "━";
	char title[] = "환 경 설 정";
	char menus[4][35] = {
		{ "기본 회전" },
		{ "속도조절 버튼(+,-)" },
		{ "프리뷰 항상 켜기" }
	};

	boolean menusSwitch[3] = { isTurnMode,isSpeedMode,isPreViewMode };
	char turn[2][10] = {
		{ "우회전" },
		{ "좌회전" }
	};
	char isOnOff[2][10] = {
		{ "ON" },
		{ "OFF" }
	};


	clrscr();	//화면 전부 지움

	// 설정창 출력
	setColor(WHITE); putsxy(16, 5, title);
	setColor(GREEN);
	for (int i = 0; i < 42; i++) {
		putsxy(16 + i, 6, rowLine);
	}

	setColor(GREEN); putsxy(16, 8, menus[0]); setColor(WHITE);

	setColor(YELLOW); putsxy(44, 8, turn[0]); setColor(WHITE);
	putchxy(51, 8, '/');
	putsxy(53, 8, turn[1]);


	putsxy(16, 10, menus[1]);
	putsxy(44, 10, isOnOff[0]);
	putchxy(48, 10, '/');
	setColor(YELLOW); putsxy(51, 10, isOnOff[1]); setColor(WHITE);


	putsxy(16, 12, menus[2]);
	setColor(YELLOW);  putsxy(44, 12, isOnOff[0]); setColor(WHITE);
	putchxy(48, 12, '/');
	putsxy(51, 12, isOnOff[1]);

	setColor(GRAY); putsxy(30, 15, "설정완료(ENTER)"); setColor(WHITE);


	while (isconfig) {

		for (int i = 0; i < 3; i++) {
			if (i == menuCount) {
				setColor(GREEN); putsxy(16, 8 + (2 * i), menus[i]);
				if (save_menuCount != -1) { setColor(WHITE); putsxy(16, 8 + (2 * save_menuCount), menus[save_menuCount]); }
			}
			if (menusSwitch[i] == TRUE) {
				if (i == 0) {
					setColor(YELLOW); putsxy(44, 8, turn[0]);
					setColor(WHITE); putsxy(53, 8, turn[1]);
				}
				else if (i == 1) {
					setColor(YELLOW); putsxy(44, 10, isOnOff[0]);
					setColor(WHITE); putsxy(51, 10, isOnOff[1]);
				}
				else if (i == 2) {
					setColor(YELLOW); putsxy(44, 12, isOnOff[0]);
					setColor(WHITE); putsxy(51, 12, isOnOff[1]);
				}
			}
			else {
				if (i == 0) {
					setColor(YELLOW); putsxy(53, 8, turn[1]);
					setColor(WHITE); putsxy(44, 8, turn[0]);
				}
				else if (i == 1) {
					setColor(YELLOW); putsxy(51, 10, isOnOff[1]);
					setColor(WHITE); putsxy(44, 10, isOnOff[0]);
				}
				else if (i == 2) {
					setColor(YELLOW); putsxy(51, 12, isOnOff[1]);
					setColor(WHITE); putsxy(44, 12, isOnOff[0]);
				}
			}
		}

		char ch = _getch();	// 키보드 입력을 받아 그에 따라 명령실행
		switch (ch) {
		case DOWN: {
			save_menuCount = menuCount;
			menuCount = menuCount == 2 ? 0 : menuCount + 1;
			break;
		}
		case UP: {
			save_menuCount = menuCount;
			menuCount = menuCount == 0 ? 2 : menuCount - 1;
			break;
		}
		case LEFT: {
			menusSwitch[menuCount] = TRUE;
			break;
		}
		case RIGHT: {
			menusSwitch[menuCount] = FALSE;
			break;
		}
		case ENTER: {
			isconfig = FALSE;
			break;
		}
		}
	}
	// 변경한설정내용을 적용한다.
	isTurnMode = menusSwitch[0];
	isSpeedMode = menusSwitch[1];
	isPreViewMode = menusSwitch[2];
}

int pausePage() {

	int menuCount = 0;
	boolean isPausePage = TRUE;
	char menus[3][24] = {
		{ "계 속 하 기" },
		{ "메 뉴 로  돌 아 가 기" }
	};

	while (isPausePage) {
		clrscr();
		if (menuCount == 0) {
			setColor(GREEN); putsxy(26, 5, menus[0]);
			setColor(WHITE); putsxy(26, 7, menus[1]);
		}
		else if (menuCount == 1) {
			setColor(WHITE); putsxy(26, 5, menus[0]);
			setColor(GREEN); putsxy(26, 7, menus[1]);
		}
		char ch = _getch();
		switch (ch) {
		case UP:case DOWN: {
			menuCount = menuCount == 0 ? 1 : 0;
			break;
		}
		case ENTER: {
			if (menuCount == 0) { // "계속하기"선택시 게임을 다시 진행한다.
				isPausePage = FALSE;
				break;
			}
			else if (menuCount == 1) {	//"메뉴로 돌아가기"선택시 1을 반환하여 메뉴로 돌아간다.
				return 1;
				break;
			}
		}

		}
	}
	return -1;
}