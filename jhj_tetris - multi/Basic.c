#include "Tetris_Header.h"


// x,y위치에 int형변수 출력
void putixy(int x, int y, const int d) { gotoxy(x, y); printf("%d", d); }
// x,y위치에 string(*char)형변수 출력
void putsxy(int x, int y, const char* c) { gotoxy(x, y); puts(c); }
// x,y위치에 cahr형변수 출력
void putchxy(int x, int y, const char c) { gotoxy(x, y); _putch(c); }

// 화면을 모두 지운다.
void clrscr() {
	system("cls");
}

// 커서를 x,y좌표로 이동시킨다.
void gotoxy(int x, int y) {
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

// 커서를 숨기거나 다시 표시한다.
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

// 화면의 크기 설정한다.
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

// 콘솔에 출력되는 색을 변경한다.
void setColor(WORD Attribute) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attribute);
}

// 블록별 색을 지정한다.
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

// 커서의 x 좌표를 조사한다.
int wherex() {
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.X;
}

// 커서의 y좌표를 조사한다.
int wherey() {
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
	return BufInfo.dwCursorPosition.Y;
}

