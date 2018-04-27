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

// ������������ �����ش�.
void showMainPage() {

	printTETRIS();	// ����ȭ�鿡 TETRIS �������

	int menu_X = 26;	// �޴�����â�� x ��ġ
	int menu_Y = 12;	// �޴�����â�� y ��ġ
	int selectedMenu = 0;

	// �޴� ����(���� ���� ����)
	for (int i = 0; i < 3; i++) {
		if (selectedMenu == i)
			setColor(GREEN);
		else
			setColor(WHITE);
		putsxy(menu_X, menu_Y + (i * 2), menus[i]);
	}

	int flag = 1;
	while (flag) {	// �޴� ����

		char ch = _getch();	//Ű���尪 1���� �޾� �޴� ����
		switch (ch) {
		case DOWN:
			if (selectedMenu >= 3)	selectedMenu = 3;	// ������ �޴� ����
			else {	// ���� �޴��� ����ڷ� �ٲٰ� ���������� �޴� ���ڸ� �ʷϻ����� ����
				setColor(WHITE);	putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
				selectedMenu++;
				setColor(GREEN);	putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
			}
			break;
		case UP:
			if (selectedMenu <= 0)	selectedMenu = 0;	// ù �޴� ����
			else {	// ���� �޴��� ����ڷ� �ٲٰ� ���������� �޴� ���ڸ� �ʷϻ����� ����
				setColor(WHITE);	putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
				selectedMenu--;
				setColor(GREEN);	putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
			}
			break;
		case ENTER:
			if (selectedMenu == 0) { mode = SINGLEMODE; gameStart(); }	// �̱۸�� ����
			else if (selectedMenu == 1) { multiSetting(); }	// ��Ƽ��� ���� ����
			else if (selectedMenu == 2) { configPage(); }	// ������� ����
			else if (selectedMenu == 3) exit(1);	// ����

			printTETRIS();	// ���θ�� �ʱ�ȭ��
			setColor(GREEN); putsxy(menu_X, menu_Y + (selectedMenu * 2), menus[selectedMenu]);
			break;
		}
	}
}

// 'TETRIS'�������
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
				if (tetris[i][j][k] == 1) printf("��");
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
	srand((unsigned)time(NULL)); // �ð����� ���� ����
	setcursortype(NOCURSOR); //Ŀ�� ������ ����
	resz(80, 24);	//�ܼ��� ũ�� ����
}
