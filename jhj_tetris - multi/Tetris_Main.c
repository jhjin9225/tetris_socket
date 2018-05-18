
#include "Tetris_Header.h"


char menus[4][30] = {
	{ "S I N G L E  P L A Y" },
	{ "M U L T I  P L A Y" },
	{ "S E T T I N G" },
	{ "E X I T" }
};

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
int mode = -1;
int winCnt = 0;
int readyCnt = 0;
int sockaddr_in_size;
int attackCnt=0;

char ip[20];
char client_num;
char playerName[3][50];
char myName[50];

boolean isTurnMode = TRUE;
boolean isSpeedMode = FALSE;
boolean isPreViewMode = FALSE;
boolean isBlockCreated = FALSE;
boolean ishold = TRUE;
boolean isConnected = TRUE;
boolean isStartGame = FALSE;
boolean isRecvQ = FALSE;
boolean msgState = FALSE;

//스레드
HANDLE thread1;
HANDLE thread2;
CRITICAL_SECTION cs;

//큐(Queue)
ArrayQueueNode newBlock;
ArrayQueue *blockQueue;

//소켓통신
WSADATA wsaData;
SOCKET listen_sock, server_sock[4], client_sock;
SOCKADDR_IN listen_addr, connect_addr[4], client_addr;
HANDLE serverThread[3], clientThread;



int main(void) {

	initMain();
	printMainPage();
	
	return 0;
}

