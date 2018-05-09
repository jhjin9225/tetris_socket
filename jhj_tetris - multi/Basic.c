#include "Tetris_Header.h"


// x,y��ġ�� int������ ���
void putixy(int x, int y, const int d) { gotoxy(x, y); printf("%d", d); }
// x,y��ġ�� string(*char)������ ���
void putsxy(int x, int y, const char* c) { gotoxy(x, y); puts(c); }
// x,y��ġ�� cahr������ ���
void putchxy(int x, int y, const char c) { gotoxy(x, y); _putch(c); }

// ȭ���� ��� �����.
void clrscr() {
	system("cls");
}

// Ŀ���� x,y��ǥ�� �̵���Ų��.
void gotoxy(int x, int y) {
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

// Ŀ���� ����ų� �ٽ� ǥ���Ѵ�.
void setcursortype(CURSOR_TYPE c) {
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

// ȭ���� ũ�� �����Ѵ�.
void resz(int w, int h) {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD temp = { 999, 999 };
	COORD size = { w,h };
	SMALL_RECT sr;

	sr.Left = 0;
	sr.Top = 0;
	sr.Right = w - 1;
	sr.Bottom = h - 1;

	SetConsoleScreenBufferSize(hout, temp);
	SetConsoleWindowInfo(hout, TRUE, &sr);
	SetConsoleScreenBufferSize(hout, size);
}

// �ֿܼ� ��µǴ� ���� �����Ѵ�.
void setColor(WORD Attribute) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attribute);
}

// ��Ϻ� ���� �����Ѵ�.
void selectColor(int block) {
	switch (block) {
	case 0: setColor(RED); break;
	case 1: setColor(BLUE); break;
	case 2: setColor(BROWN); break;
	case 3: setColor(GREEN); break;
	case 4: setColor(PINK); break;
	case 5: setColor(YELLOW); break;
	case 6: setColor(SKYBLUE); break;
	default: setColor(WHITE); break;
	}
}

// Ŀ���� x ��ǥ�� �����Ѵ�.
int wherex() {
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.X;
}

// Ŀ���� y��ǥ�� �����Ѵ�.
int wherey() {
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.Y;
}

