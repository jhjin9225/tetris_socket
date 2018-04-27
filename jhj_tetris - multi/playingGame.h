#pragma once
#include "tools.h"
#include "SelectMulMode.h"


#define PORT 1234

void gameStart() {

	initGame();	// 게임실행 기본설정
	
	if (ISSINGLE) {
		printGameStart();	// 게임 대기화면 출력
		getchar();	// 엔터를 누를때까지 대기
		printStage();	// 블록이 내려오는 화면 출력(printfGameStart를 없애기 위해)
	}
	else {
		txSock("ready");	// 준비완료 문자를 보낸다.
		while (readyCnt != clientCount) delay(1);	// 다른 플레이어가 준비될때까지 대기한다.
	}
	InitializeCriticalSection(&cs);	// 임계영역 설정
	thread1 = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)autoDownBlock, NULL, 0, NULL);	// 시간에 따라 블럭이 떨어지는 쓰레드
	thread2 = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)listenMsg, NULL, 0, NULL);	// 점수를 표시되면 일정시간후 지워주는 쓰레드
	playing();	// 실제로 게임이 실행


	WaitForSingleObject(thread1, INFINITE); // 쓰레드가 종료될떄까지 대기.
	WaitForSingleObject(thread2, INFINITE);

	while (_getch() != 13) { delay(50); }	// 엔터를 누루때까지 대기

	blockQueue = deleteQueue(blockQueue);	//큐를 제거한다.

	if (ISSERVER) {
		clrscr();
		for (int i = 0; i < clientCount; i++) {
			TerminateThread(serverThread[i], 0);	// 스레드 강제종료
			WaitForSingleObject(serverThread[i], INFINITE);
		}
	}
	else if (ISCLIENT) {
		clrscr();
		TerminateThread(clientThread, 0);	//스레드 강제종료
		WaitForSingleObject(clientThread, INFINITE);
	}

	DeleteCriticalSection(&cs);	// 임계영역 종료
}

// 서버, 클라이언트를 선택하는 페이지
void multiSetting() {
	clrscr();
	setColor(WHITE);	printf("사용자의 이름을 입력하세요 : ");
	scanf("%s", &myName);

	int selectedMenu = 0;
	int menu_X = 26;	// 메뉴선택창의 x 위치
	int menu_Y = 5;	// 메뉴선택창의 y 위치
	boolean isMultiPage = TRUE;
	char menus[4][24] = {
		{ "방 만 들 기" },
	{ "방 접 속 하 기" },
	{ "메 뉴 로  돌 아 가 기" }
	};

	setColor(GREEN); putsxy(26, 5, menus[0]);
	setColor(WHITE); putsxy(26, 7, menus[1]);
	putsxy(26, 9, menus[2]);

	while (isMultiPage) {
		char ch = _getch();
		switch (ch) {
		case DOWN:
			if (selectedMenu >= 2)	selectedMenu = 2;	// 마지막 메뉴 제한
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
			if (selectedMenu == 0) {
				server(makeRoom());
				setColor(GREEN); putsxy(26, 5, menus[0]);
				setColor(WHITE); putsxy(26, 7, menus[1]);
				setColor(WHITE); putsxy(26, 9, menus[2]);
				break;
			}
			else if (selectedMenu == 1) {
				client();
				setColor(WHITE); putsxy(26, 5, menus[0]);
				setColor(GREEN); putsxy(26, 7, menus[1]);
				setColor(WHITE); putsxy(26, 9, menus[2]);
				break;
			}
			else if (selectedMenu == 2) {
				isMultiPage = FALSE;
				break;
			}
		}

	}
}
// 키보드를 받아 게임이 실행되는곳이다.
void playing() {
	while (isStartGame || ISSINGLE) {

		if (_kbhit()) PlayerMoveAction();	// 키보드 입력에 따라 명령실행
		if (!isSpeedMode)checkScore();	// (스피드모드일때만) 점수에 따라 블록이 내려오는 속도 변경
		EnterCriticalSection(&cs);
		if (!isBlockCreated) createNewBlock();	// 블록이 땅에 닫으면 블록 생성
		LeaveCriticalSection(&cs);
		delay(1);
	}
}

// 키보드를 받아 동작을한다.
int PlayerMoveAction() {
	int dx = 0, dy = 0;
	char ch;

	ch = _getch();

	EnterCriticalSection(&cs);
	switch (ch) {
	case ESC:
		if (ISSINGLE) {
			if (pausePage() == 1) {	// 게임이 종료되고 메인화면으로 이동한다.
				TerminateThread(thread1, 0);
				TerminateThread(thread2, 0);
				blockQueue = deleteQueue(blockQueue);
				isStartGame = FALSE;
				LeaveCriticalSection(&cs);
				DeleteCriticalSection(&cs);
				showMainPage();
			}
			else {	// pausePage를 지우고 다시 게임화면으로 이동후 게임진행
				clrscr();
				printStage();
				printBlockSpace();
				printInfo();
				printNextBlock();
				printHoldBlock();
			}
		}
		break;

	case LEFT:	dx = -1;	break;
	case RIGHT: dx = 1;	break;
	case UP:  turnBlock(isTurnMode);	break;
	case DOWN: dy = 1;	break;
	case SPACEBAR: fastDown();	break;
	case TURNLEFT: turnBlock(isTurnMode);	break;
	case 90: case HOLD:
		if (ishold) holdingBlock();	// 블럭을 홀드한다.
		ishold = FALSE;
		break;

	case RESTART:
		if (ISSINGLE) {
			initBlock();	
			printGameStart();
			while (_getch() != 13) { delay(1); }
			printStage();
		}
		break;

	case PAUSE:
		if (ISSINGLE) {
			printGamePause();
			while (_getch() != PAUSE) { delay(1); }
			printStage();
			moveBlock(0, 0);
		}
		break;

	case LEVELUP:
		if (isSpeedMode) {
			leveltime = leveltime <= 150 ? 150 : leveltime - 100;
			printInfo();
		}
		break;

	case LEVELDOWN:
		if (isSpeedMode) {
			leveltime = leveltime >= 1000 ? 1000 : leveltime + 100;
			printInfo();
		}
		break;

	}
	if (dx != 0 || dy != 0) moveBlock(dx, dy);	// x, y 방향으로 이동했다면 블럭을 움직인다.
	LeaveCriticalSection(&cs);

}

// 점수에 따라 블럭이 떨어지는 속도를 바꾼다.
void checkScore() {	
	if (score < 1000) leveltime = 1000;
	else if (score < 2500) leveltime = 800;
	else if (score < 4000) leveltime = 600;
	else if (score < 5500) leveltime = 400;
	else if (score < 7000) leveltime = 250;
	else if (score < 10000) leveltime = 150;
}

// 블럭을 맨아래로 이동한다.
void fastDown() {
	while (isBlockCreated) {
		moveBlock(0, 1);
	}
}
