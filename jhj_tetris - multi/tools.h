#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <windows.h>
#endif
#include <process.h>

#ifndef __xlinux__
#pragma comment(lib,"ws2_32.lib")
#include<WinSock2.h>
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <windows.h>
#endif
#else
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#endif


#define ESC 27
#define SPACEBAR 32
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13
#define HOLD 122	// 'z'
#define TURNLEFT 99	// 'c'
#define RESTART 114	// 'r'
#define PAUSE 112	// 'p'
#define LEVELUP 61
#define LEVELDOWN 45
#define STARTX 24
#define STARTY 1
#define MSG_X 42
#define MSG_Y 16

#define RED (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define BLUE (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define PINK (RED | BLUE)
#define WHITE (RED | GREEN | BLUE)
#define GREEN (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define YELLOW (RED | GREEN)
#define SKYBLUE (GREEN | BLUE)
#define ORIGINAL 0x0007
#define GRAY 0x0008
#define BROWN 0x04

#define RED_BG (BACKGROUND_RED | BACKGROUND_INTENSITY)
#define BLUE_BG (BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define PINK_BG (RED_BG | BLUE_BG)
#define WHITE_BG (RED_BG | GREEN_BG | BLUE_BG)
#define GREEN_BG (BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define YELLOW_BG (RED_BG | GREEN_BG)
#define SKYBLUE_BG (GREEN_BG | BLUE_BG)

#define delay(n) Sleep(n)                              // n/1000초만큼 시간 지연
#define randomize() srand((unsigned)time(NULL))         // 난수 발생기 초기화
#define random(n) (rand() % (n))                        //0~n까지의 난수 발생

#define SERVERMODE 0
#define CLIENTMODE 1
#define SINGLEMODE 2

#define ISSERVER (mode==0)
#define ISCLIENT (mode==1)
#define ISSINGLE (mode==2)

#define Q_MAX	10
#define Q_FILL_SIZE	Q_MAX / 2

//==========================================================================//

//==========================================================================//




typedef struct {
	int element;
}ArrayQueueNode;

typedef struct {
	int maxElementCount;	// 최대 원소갯수
	int currentElementCount;	// 현재 원소 갯수
	int front;	//제거할 자료르 ㄹ가리키는 포인터
	int rear;	// 추가할 저료의 위치를 가리키는 포인터
	ArrayQueueNode *pElement;
}ArrayQueue;

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;



//thread.h
void autoDownBlock();
void listenMsg();
void serverRecvThread();
void clientRecvThread();

//queue.h
ArrayQueue *createQueue(int max);
ArrayQueueNode* deQueue(ArrayQueue* Queue);
int enQueue(ArrayQueue* Queue, ArrayQueueNode element);
int isQueueFull(ArrayQueue* Queue);
int isQueueEmpty(ArrayQueue* Queue);
ArrayQueue *deleteQueue(ArrayQueue* Queue);
ArrayQueueNode* peek(ArrayQueue *Queue);

//interface.h
void showMainPage();

void printGameOver();
void printGameWin();
void printGameStart();
void printGamePause();
void printInfo();
void printStage();
void printOtherPlayer();
void printNextBlock();
void printHoldBlock();
void printBlockSpace();
void printTETRIS();
boolean checkWalls(int block, int motion, int dx, int dy);
boolean checkPreViewWalls(int block, int motion, int x, int y, int dy);
void changeBlockToWalll();
void checkAllLine();
void clearLine(int line);
void txStageData();



//basic_function.h
void clrscr();
void gotoxy(int x, int y);
int wherex();
int wherey();
void setcursortype(CURSOR_TYPE c);
void resz(int w, int h);
void setColor(WORD Attribute);
void selectColor(int block);

void putixy(int x, int y, int d);
void putsxy(int x, int y, char* c);
void putchxy(int x, int y, char c);
void printB(int x, int y);

//pages.h
void configPage();
int pausePage();

// gameMode
void gameStart();
void multiSetting();
void playing();
int PlayerMoveAction();
void checkScore();



//initialize.h
void initGame();
void initBlock();
void initQueueBlockes();

//blockfunction
void createBlock(int x, int y, int block, int motion);
void removeBlock();
void createPreViewBlock(int x, int y, int block, int motion);
void removePreViewBlock();
void saveBlock(int x, int y, int block, int motion);
void turnBlock(boolean isRight);
void holdingBlock();
void createNewBlock();
void moveBlock(int dx, int dy);
void fastDown();
void txMovingBlock(int x, int y, int block, int motion);


//select_multi_mode
void server(int playerCnt);
void client();
int makeRoom();
void txSock(char msg[1024]);
void reflexBlocks(char msg[1024], int exception);
void reflex(char msg[1024], int from);
void txPlayers();
char* GetDefaultMyIP_str();
//==========================================================================//
//==========================================================================//



int stageData[22][12] = {
	{ 7,7,7,7,7,7,7,7,7,7,7,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,8,8,8,8,8,8,8,8,8,8,7 },
	{ 7,7,7,7,7,7,7,7,7,7,7,7 }
};
int stage[22][12];
//배열 순서대로 block, motion, block한개위치값, , xy값
int blocks[7][4][4][2] = {
	{	// ㄱ자 블록 시계방향 순으로 회전
		{ { 0,0 },{ 1,0 },{ 2,0 },{ 2,1 } },
{ { 1,0 },{ 1,1 },{ 0,2 },{ 1,2 } },
{ { 0,0 },{ 0,1 },{ 1,1 },{ 2,1 } },
{ { 0,0 },{ 1,0 },{ 0,1 },{ 0,2 } }
	},
	{	// 역 ㄱ 자 블록
		{ { 0,0 },{ 1,0 },{ 2,0 },{ 0,1 } },
{ { 0,0 },{ 1,0 },{ 1,1 },{ 1,2 } },
{ { 0,1 },{ 1,1 },{ 2,0 },{ 2,1 } },
{ { 0,0 },{ 0,1 },{ 0,2 },{ 1,2 } }
	},
	{	//  ㄹ 자 블록
		{ { 0,0 },{ 1,0 },{ 1,1 },{ 2,1 } },
{ { 1,0 },{ 0,1 },{ 1,1 },{ 0,2 } },
{ { 0,0 },{ 1,0 },{ 1,1 },{ 2,1 } },
{ { 1,0 },{ 0,1 },{ 1,1 },{ 0,2 } }
	},
	{	//  역 ㄹ 자 블록
		{ { 1,0 },{ 2,0 },{ 0,1 },{ 1,1 } },
{ { 0,0 },{ 0,1 },{ 1,1 },{ 1,2 } },
{ { 1,0 },{ 2,0 },{ 0,1 },{ 1,1 } },
{ { 0,0 },{ 0,1 },{ 1,1 },{ 1,2 } }
	},
	{	//  ㅗ 자 블록
		{ { 1,0 },{ 0,1 },{ 1,1 },{ 2,1 } },
{ { 0,0 },{ 0,1 },{ 0,2 },{ 1,1 } },
{ { 0,0 },{ 1,0 },{ 2,0 },{ 1,1 } },
{ { 0,1 },{ 1,1 },{ 1,0 },{ 1,2 } }
	},
	{	//  ㅁ 자 블록
		{ { 0,0 },{ 0,1 },{ 1,0 },{ 1,1 } },
{ { 0,0 },{ 0,1 },{ 1,0 },{ 1,1 } },
{ { 0,0 },{ 0,1 },{ 1,0 },{ 1,1 } },
{ { 0,0 },{ 0,1 },{ 1,0 },{ 1,1 } }
	},
	{	//  l 자 블록
		{ { 1,0 },{ 1,1 },{ 1,2 },{ 1,3 } },
{ { 0,1 },{ 1,1 },{ 2,1 },{ 3,1 } },
{ { 1,0 },{ 1,1 },{ 1,2 },{ 1,3 } },
{ { 0,1 },{ 1,1 },{ 2,1 },{ 3,1 } }
	}
};

ArrayQueueNode newBlock;
ArrayQueue *blockQueue;

int holdBlock = -1;
int holdMotion = 0;
int save_x = STARTX;
int save_y = STARTY;
int save_block = 0;
int save_motion = 0;
int save_pv_y = 0;
int save_pv_x = 0;
int score = 0;
int combo = 0;
int nextBlock[2] = { 0,0 };
int leveltime = 1000;

int clientCount;

WORD saveColor = 0;
boolean isTurnMode = TRUE;
boolean isSpeedMode = FALSE;
boolean isPreViewMode = FALSE;
boolean msgState = FALSE;
boolean isBlockCreated = FALSE;
boolean ishold = TRUE;
boolean isConnected = TRUE;

HANDLE thread1;
HANDLE thread2;
CRITICAL_SECTION cs;

HANDLE hd;

int mode = -1;
boolean isStartGame= FALSE;
boolean isRecvQ = FALSE;

char client_num;
int winCnt = 0; 
int readyCnt = 0;
char playerName[3][50];
char myName[50];