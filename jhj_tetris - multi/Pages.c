#include "Tetris_Header.h"


extern boolean isTurnMode;
extern boolean isSpeedMode;
extern boolean isPreViewMode;

// ������������ �����ְ� ������ �����Ѵ�.
void configPage() {

	int menuCount = 0;
	int save_menuCount = -1;
	boolean isconfig = TRUE;
	boolean isTurn = TRUE;
	boolean isSpeed = FALSE;
	boolean isPreView = FALSE;

	char rowLine[] = "��";
	char title[] = "ȯ �� �� ��";
	char menus[4][35] = {
		{ "�⺻ ȸ��" },
		{ "�ӵ����� ��ư(+,-)" },
		{ "������ �׻� �ѱ�" }
	};

	boolean menusSwitch[3] = { isTurnMode,isSpeedMode,isPreViewMode };
	char turn[2][10] = {
		{ "��ȸ��" },
		{ "��ȸ��" }
	};
	char isOnOff[2][10] = {
		{ "ON" },
		{ "OFF" }
	};


	clrscr();	//ȭ�� ���� ����

	// ����â ���
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

	setColor(GRAY); putsxy(30, 15, "�����Ϸ�(ENTER)"); setColor(WHITE);


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

		char ch = _getch();	// Ű���� �Է��� �޾� �׿� ���� ��ɽ���
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
	// �����Ѽ��������� �����Ѵ�.
	isTurnMode = menusSwitch[0];
	isSpeedMode = menusSwitch[1];
	isPreViewMode = menusSwitch[2];
}

int pausePage() {

	int menuCount = 0;
	boolean isPausePage = TRUE;
	char menus[3][24] = {
		{ "�� �� �� ��" },
		{ "�� �� ��  �� �� �� ��" }
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
			if (menuCount == 0) { // "����ϱ�"���ý� ������ �ٽ� �����Ѵ�.
				isPausePage = FALSE;
				break;
			}
			else if (menuCount == 1) {	//"�޴��� ���ư���"���ý� 1�� ��ȯ�Ͽ� �޴��� ���ư���.
				return 1;
				break;
			}
		}

		}
	}
	return -1;
}