#pragma once
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
#define ATTACK 120 // 'x'

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
#define ISMULTI	 (mode!=2)

#define Q_MAX	200
#define Q_FILL_SIZE	Q_MAX / 2

#define ATTACK_X 7
#define ATTACK_Y 14



//=============구조체, 열거형=============//

typedef struct {
	int element;
}ArrayQueueNode;

typedef struct {
	int maxElementCount;	// 최대 원소갯수
	int currentElementCount;	// 현재 원소 갯수
	int front;	//제거할 자료를 가리키는 포인터
	int rear;	// 추가할 저료의 위치를 가리키는 포인터
	ArrayQueueNode *pElement;
}ArrayQueue;

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;



//============구조체, 열거형============//

//basic.h
void putixy(int x, int y, int d);
void putsxy(int x, int y, char* c);
void putchxy(int x, int y, char c);
void clrscr();
void gotoxy(int x, int y);
void setcursortype(CURSOR_TYPE c);
void resz(int w, int h);
void setColor(WORD Attribute);
void selectColor(int block);
int wherex();
int wherey();


//blockes.c
void createBlock(int x, int y, int block, int motion);
void removeBlock();
void createPreViewBlock(int x, int y, int block, int motion);
void removePreViewBlock();
void saveBlock(int x, int y, int block, int motion);
void turnBlock(boolean isRight);
void createNewBlock();
void moveBlock(int dx, int dy);
void holdingBlock();
void changeBlockToWalll();
void checkAllLine();
void clearLine(int line);
boolean checkWalls(int block, int motion, int dx, int dy);
boolean checkPreViewWalls(int block, int motion, int x, int y, int dy);
void attacked();
void addScore(int sc);


//initialize.c
void initMain();
void initGame();
void initBlock();
void initQueueBlockes();


//interface.h
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
void printMainPage();


//pages.h
void configPage();
int pausePage();


// gameMode
void gameStart();
void multiSetting();
void playing();
void keyboardAction();
void checkScore();
void fastDown();


//queue.h
ArrayQueue *createQueue(int max);
ArrayQueueNode* deQueue(ArrayQueue* Queue);
void enQueue(ArrayQueue* Queue, ArrayQueueNode element);
int isQueueFull(ArrayQueue* Queue);
int isQueueEmpty(ArrayQueue* Queue);
ArrayQueue *deleteQueue(ArrayQueue* Queue);
ArrayQueueNode* peek(ArrayQueue *Queue);


//selectMultiMode
void server(int playerCnt);
void client();
int makeRoom();
char* GetDefaultMyIP_str();
void txPlayers();
void txSock(char msg[1024]);
void reflexBlocks(char msg[1024], int exception);
void reflex(char msg[1024], int from);
void txMovingBlock(int x, int y, int block, int motion);
void txStageData();


//thread.h
void serverRecvThread();
void clientRecvThread();
void autoDownBlock();
void listenMsg();